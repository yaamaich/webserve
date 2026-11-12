/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 15:59:38 by emad              #+#    #+#             */
/*   Updated: 2026/10/21 21:13:50 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/LocationBuilder.hpp"
#include <stdexcept>

LocationBuilder::LocationBuilder() {};

void    LocationBuilder::setPath(const std::string& path)
{
    _config._path = path;
}

void    LocationBuilder::setRoot(const std::string& root)
{
    _config._root = root;
}

void    LocationBuilder::setIndex(const std::string& index)
{
    _config._index = index;
}

void    LocationBuilder::setAutoIndex(bool state)
{
    _config._autoindex = state;
}

void    LocationBuilder::setRedirect(const std::string& redirect)
{
    _config._redirect = redirect;
}

void    LocationBuilder::setUploadStore(const std::string& store)
{
    _config._uploadStor = store;
}

void    LocationBuilder::clearMethoD()
{
    _config._allowedMethods.clear();
}

void    LocationBuilder::addMethod(const std::string& method)
{
    if (method != "GET" && method != "POST" && method != "DELETE")
        throw std::runtime_error("Config Error: Unsupported HTTP method: " + method);
    _config._allowedMethods.push_back(method);
}

void    LocationBuilder::addCgi(const std::string& ext, const std::string& execPath)
{
    _config._cgi[ext] = execPath;
}

LocationConfig  LocationBuilder::build() const
{
    if (_config._path.empty())
        throw std::runtime_error("Config Error: Location path cannot be empty");
    return (_config);
}
