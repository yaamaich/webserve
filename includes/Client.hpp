/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imatouil <imatouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 18:54:47 by imatouil          #+#    #+#             */
/*   Updated: 2026/10/20 11:19:31 by imatouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <cstddef>
#include <sys/socket.h>
#include <unistd.h>

class Client
{
	private:
		int			_fd;
		std::string	_in;
		std::string	_out;
		ssize_t		_sent;
	public:
		Client();
		explicit Client(int clinetFd);
		Client(const Client& obj);
		Client&	operator=(const Client& obj);
		~Client();

		int	getFd() const;
		const std::string& getIn() const;
		
		bool	hasPendingOut() const;
		bool	readFromSocket();
		bool	writeToSocket();
		bool	isRequestComplet() const;
		bool	isResponseSent() const;
		void	setResponse(const std::string& resp);
};

#endif