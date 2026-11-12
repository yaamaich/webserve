/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBuilder.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 16:49:26 by emad              #+#    #+#             */
/*   Updated: 2026/10/21 17:03:19 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBUILDER_HPP
#define SERVERBUILDER_HPP

#include "ConfigData.hpp"
#include <string>

class ServerBuilder
{
    private:
        ServerConfig    _config;
    public:
        ServerBuilder();
        void    setHostAndPort(const std::string& host, int port);
        void    addServerName(const std::string& name);
        void    setMaxBodySize(size_t maxSize);
        void    addErrorPages(int code, const std::string& path);
        void    addLocation(const LocationConfig& location);

        ServerConfig    build() const;
};

#endif
