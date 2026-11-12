/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 17:10:53 by emad              #+#    #+#             */
/*   Updated: 2026/10/21 19:00:13 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigParser.hpp"
#include <cstdlib>
#include <stdexcept>

ConfigParser::ConfigParser()
{
    regesterHandlers();
}

void    ConfigParser::regesterHandlers()
{
    server_handlers["listen"] = &ConfigParser::handleListen;
    server_handlers["server_name"] = &ConfigParser::handleServerName;
    server_handlers["client_max_body_size"] = &ConfigParser::handleBodySize;
    server_handlers["error_page"] = &ConfigParser::handleErrorPage;
    server_handlers["location"] = &ConfigParser::handleLocation;

    location_handlers["root"] = &ConfigParser::handleRoot;
    location_handlers["index"] = &ConfigParser::handleIndex;
    location_handlers["autoindex"] = &ConfigParser::handleAutoIndex;
    location_handlers["allow_methods"] = &ConfigParser::handleMethods;
    location_handlers["return"] = &ConfigParser::handleReturn;
    location_handlers["upload_store"] = &ConfigParser::handleUploadStore;
    location_handlers["cgi"] = &ConfigParser::handleCgi;
}

void    ConfigParser::handleListen(TokenStream& tStream, ServerBuilder& sBuilder)
{
    std::string val = tStream.next();
    size_t colon = val.find(':');
    std::string host = "0.0.0.0";
    int port = 0;
    if (colon != std::string::npos)
    {
        host = val.substr(0, colon);
        port = std::atoi(val.substr(colon + 1).c_str());
    }
    else
        port = std::atoi(val.c_str());
    sBuilder.setHostAndPort(host, port);
    tStream.expect(";");
}

void    ConfigParser::handleServerName(TokenStream& tStream, ServerBuilder& sBuilder)
{
    while (tStream.peek() != ";")
        sBuilder.addServerName(tStream.next());
    tStream.expect(";");
}

void ConfigParser::handleBodySize(TokenStream &tStream, ServerBuilder &sBuilder) {
    sBuilder.setMaxBodySize(static_cast<size_t>(std::atol(tStream.next().c_str())));
    tStream.expect(";");
}

void    ConfigParser::handleErrorPage(TokenStream& tStream, ServerBuilder& sBuilder)
{
    int code = std::atoi(tStream.next().c_str());
    std::string path = tStream.next();
    sBuilder.addErrorPages(code, path);
    tStream.expect(";");
}

void    ConfigParser::handleLocation(TokenStream& tStream, ServerBuilder& sBuilder)
{
    LocationBuilder locBuild;
    locBuild.setPath(tStream.next());
    parseLocationBlock(tStream, locBuild);
    sBuilder.addLocation(locBuild.build());
}

void    ConfigParser::handleRoot(TokenStream& tStream, LocationBuilder& lBuilder)
{
    lBuilder.setRoot(tStream.next());
    tStream.expect(";");
}

void    ConfigParser::handleIndex(TokenStream& tStream, LocationBuilder& lBuilder)
{
    lBuilder.setIndex(tStream.next());
    tStream.expect(";");
}

void    ConfigParser::handleAutoIndex(TokenStream& tStream, LocationBuilder& lBuilder)
{
    lBuilder.setAutoIndex(tStream.next() == "on");
    tStream.expect(";");
}

void    ConfigParser::handleMethods(TokenStream& tStream, LocationBuilder& lBuilder)
{
    lBuilder.clearMethoD();
    while (tStream.peek() != ";")
        lBuilder.addMethod(tStream.next());
    tStream.expect(";");
}

void    ConfigParser::handleReturn(TokenStream& tStream, LocationBuilder& lBuilder)
{
    lBuilder.setRedirect(tStream.next());
    tStream.expect(";");
}

void    ConfigParser::handleUploadStore(TokenStream& tStream, LocationBuilder& lBuilder)
{
    lBuilder.setUploadStore(tStream.next());
    tStream.expect(";");
}

void    ConfigParser::handleCgi(TokenStream& tStream, LocationBuilder& lBuilder)
{
    std::string ext = tStream.next();
    std::string exec = tStream.next();
    lBuilder.addCgi(ext, exec);
    tStream.expect(";");
}

void    ConfigParser::parseServerBlock(TokenStream& tStream, ServerBuilder& sBuilder)
{
    tStream.expect("{");
    while (tStream.peek() != "}")
    {
        std::string directive = tStream.next();
        std::map<std::string, ServerDirectiveHandler>::iterator it = server_handlers.find(directive);
        if (it == server_handlers.end())
            throw std::runtime_error("Config Error: Unknown server directive: " + directive);
        (this->*(it->second))(tStream, sBuilder);
    }
    tStream.expect("}");
}

void    ConfigParser::parseLocationBlock(TokenStream& tStream, LocationBuilder& lBuilder)
{
    tStream.expect("{");
    while (tStream.peek() != "}")
    {
        std::string directive = tStream.next();
        std::map<std::string, LocationDirectiveHandler>::iterator it = location_handlers.find(directive);
        if (it == location_handlers.end())
            throw std::runtime_error("Config Error: Unknown location directive: " + directive);
        (this->*(it->second))(tStream, lBuilder);
    }
    tStream.expect("}");
}

std::vector<ServerConfig>   ConfigParser::parse(const std::string& filePath)
{
    TokenStream tStream;
    tStream.loadFromFile(filePath);
    std::vector<ServerConfig> servers;
    while (tStream.hasNext())
    {
        std::string block = tStream.next();
        if (block != "server")
            throw std::runtime_error("Config Error: Unexpected top-level token: " + block);
        ServerBuilder sBuilder;
        parseServerBlock(tStream, sBuilder);
        servers.push_back(sBuilder.build());
    }
    if (servers.empty())
        throw std::runtime_error("Config Error: No server blocks found in configuration file");
    return (servers);
}
