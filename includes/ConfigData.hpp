/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 13:04:26 by imatouil          #+#    #+#             */
/*   Updated: 2026/10/21 16:17:43 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGDATA_HPP
#define CONFIGDATA_HPP

#include <string>
#include <vector>
#include <map>
#include <cstddef>

struct LocationConfig
{
	std::string							_path;
	std::vector<std::string>			_allowedMethods;
	std::string							_root;
	std::string							_index;
	bool								_autoindex;
	std::string							_redirect;
	std::string							_uploadStor;
	std::map<std::string, std::string>	_cgi;
	LocationConfig();
};

struct ServerConfig
{
	std::string					_host;
	int							_port;
	std::vector<std::string>	_serverNames;
	size_t						_clientMaxBodySize;
	std::map<int, std::string>	_errorPages;
	std::vector<LocationConfig>	_locatios;
	ServerConfig();
};

#endif
