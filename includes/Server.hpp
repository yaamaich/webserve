/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 16:21:36 by imatouil          #+#    #+#             */
/*   Updated: 2026/10/22 12:55:15 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <poll.h>
#include "Client.hpp"
#include <iostream>

#include "ConfigData.hpp"

#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
	private:
		std::vector<ServerConfig>	_configs;
		std::vector<int>			_listeners;
		int							_port;
		std::vector<struct pollfd>	_fds;
		std::map<int, Client>		_clients;

		Server(const Server &);
		Server& operator+(const Server &);

		void	acceptClient(int listnerFd);
		void	handelRead(size_t i, bool &closeIt);
		void	handelWrite(size_t i, bool &closeIt);
		void	removeClient(size_t i);
		void	addPollFd(int fd, short events);
		bool	isListener(int fd) const;
	public:
		// explicit Server(int port);
		explicit Server(const std::vector<ServerConfig> &configs);
		~Server();
		
		bool	init(); // socket, bind, listen, non-blocking
		void	run(); // poll loop
};

#endif