/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 17:11:13 by emad              #+#    #+#             */
/*   Updated: 2026/10/21 17:26:41 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "ConfigData.hpp"
#include "TokenStream.hpp"
#include "ServerBuilder.hpp"
#include "LocationBuilder.hpp"
#include <map>
#include <vector>

class ConfigParser
{
    private:
        typedef void (ConfigParser::*ServerDirectiveHandler)(TokenStream &, ServerBuilder &);
        typedef void (ConfigParser::*LocationDirectiveHandler)(TokenStream &, LocationBuilder &);
        std::map<std::string, ServerDirectiveHandler> server_handlers;
        std::map<std::string, LocationDirectiveHandler> location_handlers;

        void    regesterHandlers();
        
        // Server directive handlers
        void    handleListen(TokenStream& tStream, ServerBuilder& sBuilder);
        void    handleServerName(TokenStream &tStream, ServerBuilder &sBuilder);
        void    handleBodySize(TokenStream &tStream, ServerBuilder &sBuilder);
        void    handleErrorPage(TokenStream &tStream, ServerBuilder &sBuilder);
        void    handleLocation(TokenStream &tStream, ServerBuilder &sBuilder);
        // Location directive handlers

        void    handleRoot(TokenStream& tStream, LocationBuilder& lBuilder);
        void    handleIndex(TokenStream& tStream, LocationBuilder& lBuilder);
        void    handleAutoIndex(TokenStream& tStream, LocationBuilder& lBuilder);
        void    handleMethods(TokenStream& tStream, LocationBuilder& lBuilder);
        void    handleReturn(TokenStream& tStream, LocationBuilder& lBuilder);
        void    handleUploadStore(TokenStream& tStream, LocationBuilder& lBuilder);
        void    handleCgi(TokenStream& tStream, LocationBuilder& lBuilder);
        
        void    parseServerBlock(TokenStream& tStream, ServerBuilder& sBuilder);
        void    parseLocationBlock(TokenStream& tStream, LocationBuilder& lBuilder);

    public:
        ConfigParser();
        std::vector<ServerConfig> parse(const std::string& filePath);
};

#endif
