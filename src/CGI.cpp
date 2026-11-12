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
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

CGI::CGI() {}
CGI::~CGI() {}

static std::string readAllFromPipe(int fd)
{
    std::string out;
    char buffer[4096];
    ssize_t n;
    while ((n = read(fd, buffer, sizeof(buffer))) > 0)
        out.append(buffer, n);
    return out;
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
    env.push_back("CONTENT_LENGTH=" + std::to_string(body.size()));
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env.push_back("REDIRECT_STATUS=200");

    char** envp = new char*[env.size() + 1];
    for (size_t i = 0; i < env.size(); ++i)
    {
        envp[i] = strdup(env[i].c_str());
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
    int inPipe[2];
    int outPipe[2];
    if (pipe(inPipe) < 0 || pipe(outPipe) < 0)
        return "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";

    pid_t pid = fork();
    if (pid == 0)
    {
        dup2(inPipe[0], STDIN_FILENO);
        dup2(outPipe[1], STDOUT_FILENO);

        close(inPipe[0]);
        close(inPipe[1]);
        close(outPipe[0]);
        close(outPipe[1]);

        char* argv[] = {
            strdup(interpreter.c_str()),
            strdup(scriptPath.c_str()),
            NULL
        };

        char** envp = buildEnvironment(scriptPath, method, queryString, body, contentType);

        execve(interpreter.c_str(), argv, envp);

        _exit(1);
    }

    close(inPipe[0]);
    close(outPipe[1]);

    if (!body.empty())
        write(inPipe[1], body.c_str(), body.size());

    close(inPipe[1]);

    int status = 0;
    waitpid(pid, &status, 0);

    std::string output = readAllFromPipe(outPipe[0]);
    close(outPipe[0]);

    if (WIFEXITED(status) && WIFEXITED(status) && WEXITSTATUS(status) == 0)
        return output;

    return "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
}