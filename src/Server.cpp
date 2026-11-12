/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 18:08:08 by imatouil          #+#    #+#             */
/*   Updated: 2026/10/22 13:14:12 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

// Server::Server(int port) : _port(port), _listener(-1) {};

Server::Server(const std::vector<ServerConfig> &configs) : _configs(configs) {}

Server::~Server()
{
	for (size_t i = 0; i < _fds.size(); i++)
		close(_fds[i].fd); // listeners and clients
}

bool	Server::init()
{
	if (_configs.empty())
		return (false);
	for (size_t i = 0; i < _configs.size(); i++)
	{
		int	listener = socket(AF_INET, SOCK_STREAM, 0);
		if (listener < 0)
		{
			std::cerr << "Error: socket() creattion falied" << std::endl;
			return (false);
		}
		int opt = 1;
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		struct sockaddr_in addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
		if (_configs[i]._host.empty() || _configs[i]._host == "0.0.0.0")
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
		else
			addr.sin_addr.s_addr = inet_addr(_configs[i]._host.c_str());
		addr.sin_port = htons(_configs[i]._port);

        if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0
            || listen(listener, 128) < 0
            || fcntl(listener, F_SETFL, O_NONBLOCK) < 0)
        {
            std::cerr << "Error: bind/listen failed on port " << _configs[i]._port << std::endl;
            close(listener);
            return false;
        }
		_listeners.push_back(listener);
        addPollFd(listener, POLLIN);
        std::cout << "Listening on " << _configs[i]._host << ":" << _configs[i]._port << std::endl;
	}
	return (true);
}

void	Server::addPollFd(int fd, short events)
{
	struct pollfd p;
	p.fd = fd;
	p.events = events;
	p.revents = 0;
	_fds.push_back(p);
}

void	Server::acceptClient(int listenerFd)
{
	int cfd = accept(listenerFd, NULL, NULL);
	if (cfd < 0)
		return ;
	if (fcntl(cfd, F_SETFL, O_NONBLOCK) < 0)
	{
		close(cfd);
		return;
	}
	addPollFd(cfd, POLLIN);
	_clients[cfd] = Client(cfd);
	std::cout << "new client fd: " << cfd << std::endl;
}

void	Server::removeClient(size_t i)
{
	int fd = _fds[i].fd;
	std::cout << "close client fd: " << fd << std::endl;
	close(fd);
	_clients.erase(fd);
	_fds.erase(_fds.begin() + i);
}

void	Server::handelRead(size_t i, bool &closeIT)
{
	Client	&c = _clients[_fds[i].fd];
	if (!c.readFromSocket())
	{
		closeIT = true;
		return;
	}
	if (c.isRequestComplet())
	{
		c.setResponse(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 5\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Hello");
		_fds[i].events = POLLOUT;
	}
}

void	Server::handelWrite(size_t i, bool &closeIt)
{
	Client &c = _clients[_fds[i].fd];
	if (!c.writeToSocket())
	{
		closeIt = true;
		return;
	}
	if (c.isResponseSent())
		closeIt = true;
}

void	Server::run()
{
	while (true)
	{
		if (poll(&_fds[0], _fds.size(), -1) < 0)
			break;
		for (size_t i = 0; i < _fds.size(); )
        {
            short re = _fds[i].revents;
            if (re == 0)
            {
                ++i;
                continue;
            }
            if (isListener(_fds[i].fd))
            {
                if (re & POLLIN)
                    acceptClient(_fds[i].fd);
                ++i;
                continue;
            }
            bool closeIt = false;
            if (re & (POLLERR | POLLNVAL))
                closeIt = true;
            else if (re & POLLIN)
                handelRead(i, closeIt);
            else if (re & POLLOUT)
                handelWrite(i, closeIt);
            else if (re & POLLHUP)
                closeIt = true;

            if (closeIt)
                removeClient(i);   // matzidch ++i
            else
                ++i;
        }
	}
}

bool	Server::isListener(int fd) const
{
	for (size_t i = 0; i < _listeners.size(); i++)
	{
		if (_listeners[i] == fd)
			return (true);
	}
	return (false);
}
