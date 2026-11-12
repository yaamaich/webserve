/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imatouil <imatouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 22:33:33 by imatouil          #+#    #+#             */
/*   Updated: 2026/10/20 11:26:10 by imatouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client() : _fd(0), _sent(0) {};

Client::Client(int fd) : _fd(fd), _sent(0) {}

Client::Client(const Client& obj) :
		_fd(obj._fd), _in(obj._in), _out(obj._out), _sent(obj._sent) {};

Client&	Client::operator=(const Client& obj)
{
	if (this != &obj)
	{
		_fd = obj._fd;
		_in = obj._in;
		_out = obj._out;
		_sent = obj._sent;
	}
	return (*this);
}

Client::~Client() {};

int	Client::getFd() const
{
	return (_fd);
}

const std::string&	Client::getIn() const
{
	return (_in);
}

bool	Client::hasPendingOut() const
{
	return (_sent < (ssize_t)_out.size());
}

bool	Client::readFromSocket()
{
	char buffer[4096];
	ssize_t	n = recv(_fd, buffer, sizeof(buffer), 0);
	if (n <= 0)
		return (false);
	_in.append(buffer, n);
	return (true);
}

bool	Client::writeToSocket()
{
	ssize_t	n = send(_fd, _out.c_str() + _sent, _out.size() - _sent, 0);
	if (n <= 0)
		return (false);
	_sent += n;
	return (true);
}

bool	Client::isRequestComplet() const
{
	return (_in.find("\r\n\r\n") != std::string::npos);
}

bool	Client::isResponseSent() const
{
	return (!_out.empty() && _sent >= (ssize_t)_out.size());
}

void	Client::setResponse(const std::string& resp)
{
	_out = resp;
	_sent = 0;
}
