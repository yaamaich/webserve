/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 13:04:24 by imatouil          #+#    #+#             */
/*   Updated: 2026/10/21 16:17:29 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigData.hpp"

LocationConfig::LocationConfig() :
				_path(""),
				_root("./www"),
				_index("index.html"),
				_autoindex(false),
				_redirect(""),
				_uploadStor("") {
				_allowedMethods.push_back("GET"); }

ServerConfig::ServerConfig() :
				_host("0.0.0.0"),
				_port(8080),
				_clientMaxBodySize(1024 * 1024) {}
