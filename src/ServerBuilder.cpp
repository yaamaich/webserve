/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBuilder.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 16:49:20 by emad              #+#    #+#             */
/*   Updated: 2026/10/21 18:55:55 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerBuilder.hpp"
#include <stdexcept>
#include <sstream>

ServerBuilder::ServerBuilder() {};

void    ServerBuilder::setHostAndPort(const std::string& host, int port)
{
    if (port <= 0 || port > 65535)
    {
        std::stringstream ss;
        ss << port;
        throw std::runtime_error("Config Error: Invalid port number: " + ss.str());
    }
    _config._host = host;
    _config._port = port;
}

void    ServerBuilder::addServerName(const std::string& name)
{
    _config._serverNames.push_back(name);
}

void    ServerBuilder::setMaxBodySize(size_t maxSize)
{
    _config._clientMaxBodySize = maxSize;
}

void    ServerBuilder::addErrorPages(int code, const std::string& path)
{
    if (code < 300 || code > 599)
        throw std::runtime_error("Config Error: Invalid HTTP error code for error_page");
    _config._errorPages[code] = path;
}

void    ServerBuilder::addLocation(const LocationConfig& location)
{
    _config._locatios.push_back(location);
}

ServerConfig    ServerBuilder::build() const
{
    return (_config);
}
