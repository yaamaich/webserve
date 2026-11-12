/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 20:01:11 by yaamaich          #+#    #+#             */
/*   Updated: 2026/11/12 20:01:11 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CGI.hpp"
#include <cerrno>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

CGI::CGI() {}
CGI::~CGI() {}

static std::string makeErrorResponse(int status, const std::string& reason)
{
    std::ostringstream response;
    response << "HTTP/1.1 " << status << " " << reason
             << "\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
    return response.str();
}

static std::string sizeToString(size_t size)
{
    std::ostringstream value;
    value << size;
    return value.str();
}

static std::string lowerCase(const std::string& value)
{
    std::string result(value);
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = static_cast<char>(std::tolower(
            static_cast<unsigned char>(result[i])));
    return result;
}

static bool isValidHeaderName(const std::string& name)
{
    if (name.empty())
        return false;
    for (size_t i = 0; i < name.size(); ++i)
    {
        unsigned char c = static_cast<unsigned char>(name[i]);
        if (!std::isalnum(c) && c != '!' && c != '#' && c != '$'
            && c != '%' && c != '&' && c != '\'' && c != '*'
            && c != '+' && c != '-' && c != '.' && c != '^'
            && c != '_' && c != '`' && c != '|' && c != '~')
            return false;
    }
    return true;
}

static std::string trim(const std::string& value)
{
    size_t first = value.find_first_not_of(" \t");
    if (first == std::string::npos)
        return "";
    size_t last = value.find_last_not_of(" \t");
    return value.substr(first, last - first + 1);
}

static bool isHopByHopHeader(const std::string& name)
{
    const std::string lower = lowerCase(name);
    return lower == "connection" || lower == "keep-alive"
        || lower == "proxy-authenticate" || lower == "proxy-authorization"
        || lower == "te" || lower == "trailer"
        || lower == "transfer-encoding" || lower == "upgrade"
        || lower == "content-length";
}

static std::string makeHttpResponse(const std::string& output)
{
    size_t separator = output.find("\r\n\r\n");
    size_t separatorSize = 4;
    size_t lfSeparator = output.find("\n\n");
    if (separator == std::string::npos
        || (lfSeparator != std::string::npos && lfSeparator < separator))
    {
        separator = lfSeparator;
        separatorSize = 2;
    }
    if (separator == std::string::npos)
        return makeErrorResponse(502, "Bad Gateway");

    const std::string rawHeaders = output.substr(0, separator);
    const std::string body = output.substr(separator + separatorSize);
    std::string headers;
    int status = 200;
    std::string reason = "OK";
    bool sawStatus = false;

    size_t start = 0;
    while (start < rawHeaders.size())
    {
        size_t end = rawHeaders.find('\n', start);
        if (end == std::string::npos)
            end = rawHeaders.size();
        std::string line = rawHeaders.substr(start, end - start);
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        start = (end == rawHeaders.size()) ? end : end + 1;
        if (line.empty())
            continue;

        size_t colon = line.find(':');
        if (colon == std::string::npos)
            return makeErrorResponse(502, "Bad Gateway");
        std::string name = line.substr(0, colon);
        std::string value = trim(line.substr(colon + 1));
        if (!isValidHeaderName(name))
            return makeErrorResponse(502, "Bad Gateway");
        for (size_t i = 0; i < value.size(); ++i)
        {
            unsigned char c = static_cast<unsigned char>(value[i]);
            if ((c < 32 && c != '\t') || c == 127)
                return makeErrorResponse(502, "Bad Gateway");
        }

        if (lowerCase(name) == "status")
        {
            if (sawStatus || value.size() < 3
                || !std::isdigit(static_cast<unsigned char>(value[0]))
                || !std::isdigit(static_cast<unsigned char>(value[1]))
                || !std::isdigit(static_cast<unsigned char>(value[2]))
                || (value.size() > 3 && value[3] != ' ' && value[3] != '\t'))
                return makeErrorResponse(502, "Bad Gateway");
            status = (value[0] - '0') * 100
                + (value[1] - '0') * 10 + (value[2] - '0');
            if (status < 100 || status > 599)
                return makeErrorResponse(502, "Bad Gateway");
            reason = trim(value.substr(3));
            if (reason.empty())
                reason = "CGI Response";
            sawStatus = true;
        }
        else if (!isHopByHopHeader(name))
            headers += name + ": " + value + "\r\n";
    }

    std::ostringstream response;
    response << "HTTP/1.1 " << status << " " << reason << "\r\n"
             << headers
             << "Content-Length: " << body.size()
             << "\r\nConnection: close\r\n\r\n";
    return response.str() + body;
}

char** CGI::buildEnvironment(const std::string& scriptPath,
                             const std::string& method,
                             const std::string& queryString,
                             const std::string& body,
                             const std::string& contentType)
{
    std::vector<std::string> env;
    env.push_back("REQUEST_METHOD=" + method);
    env.push_back("QUERY_STRING=" + queryString);
    env.push_back("SCRIPT_FILENAME=" + scriptPath);
    env.push_back("SCRIPT_NAME=" + scriptPath);
    env.push_back("CONTENT_TYPE=" + contentType);
    env.push_back("CONTENT_LENGTH=" + sizeToString(body.size()));
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env.push_back("REDIRECT_STATUS=200");

    char** envp = new char*[env.size() + 1];
    for (size_t i = 0; i < env.size(); ++i)
    {
        envp[i] = ::strdup(env[i].c_str());
        if (envp[i] == NULL)
        {
            while (i > 0)
                std::free(envp[--i]);
            delete[] envp;
            return NULL;
        }
    }
    envp[env.size()] = NULL;
    return envp;
}

std::string CGI::execute(const std::string& scriptPath,
                         const std::string& interpreter,
                         const std::string& method,
                         const std::string& queryString,
                         const std::string& body,
                         const std::string& contentType) const
{
    FILE* input = std::tmpfile();
    if (input == NULL)
        return makeErrorResponse(500, "Internal Server Error");

    size_t written = 0;
    while (written < body.size())
    {
        size_t count = std::fwrite(body.data() + written, 1,
                                   body.size() - written, input);
        if (count == 0)
        {
            std::fclose(input);
            return makeErrorResponse(500, "Internal Server Error");
        }
        written += count;
    }
    if (std::fflush(input) != 0 || std::fseek(input, 0, SEEK_SET) != 0)
    {
        std::fclose(input);
        return makeErrorResponse(500, "Internal Server Error");
    }

    int outPipe[2];
    if (pipe(outPipe) < 0)
    {
        std::fclose(input);
        return makeErrorResponse(500, "Internal Server Error");
    }

    char** envp = buildEnvironment(scriptPath, method, queryString,
                                   body, contentType);
    if (envp == NULL)
    {
        close(outPipe[0]);
        close(outPipe[1]);
        std::fclose(input);
        return makeErrorResponse(500, "Internal Server Error");
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        for (size_t i = 0; envp[i] != NULL; ++i)
            std::free(envp[i]);
        delete[] envp;
        close(outPipe[0]);
        close(outPipe[1]);
        std::fclose(input);
        return makeErrorResponse(500, "Internal Server Error");
    }
    if (pid == 0)
    {
        if (dup2(::fileno(input), STDIN_FILENO) < 0
            || dup2(outPipe[1], STDOUT_FILENO) < 0)
            _exit(127);
        close(outPipe[0]);
        close(outPipe[1]);
        std::fclose(input);

        char* argv[3];
        argv[0] = const_cast<char*>(interpreter.c_str());
        argv[1] = const_cast<char*>(scriptPath.c_str());
        argv[2] = NULL;
        execve(interpreter.c_str(), argv, envp);
        _exit(127);
    }

    for (size_t i = 0; envp[i] != NULL; ++i)
        std::free(envp[i]);
    delete[] envp;
    std::fclose(input);
    close(outPipe[1]);

    std::string output;
    char buffer[4096];
    ssize_t count;
    bool readError = false;
    while (true)
    {
        count = read(outPipe[0], buffer, sizeof(buffer));
        if (count > 0)
            output.append(buffer, static_cast<size_t>(count));
        else if (count == 0)
            break;
        else if (errno != EINTR)
        {
            readError = true;
            break;
        }
    }
    close(outPipe[0]);

    int childStatus = 0;
    pid_t waited;
    do
    {
        waited = waitpid(pid, &childStatus, 0);
    } while (waited < 0 && errno == EINTR);

    if (readError || waited < 0 || !WIFEXITED(childStatus)
        || WEXITSTATUS(childStatus) != 0)
        return makeErrorResponse(500, "Internal Server Error");
    return makeHttpResponse(output);
}
