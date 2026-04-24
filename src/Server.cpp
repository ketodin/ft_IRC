/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:52:35 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/24 14:34:33 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(int port, const std::string& password) :
	_epoll_fd(-1),
	_listen_sock(-1),
	_port(port),
	_password(password)
{
	LOG_INFO(this->_port);
	LOG_INFO(this->_password);
	LOG_INFO(this->_epoll_fd);
	LOG_INFO(this->_listen_sock);
	this->setupSocket();
}

Server::~Server()
{
	if (this->_listen_sock != -1)
		close(this->_listen_sock);
	if (this->_epoll_fd != -1)
		close(this->_epoll_fd);
}

void
Server::setupSocket()
{
	struct sockaddr_in addr;
	int				   opt = 1;

	this->_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_listen_sock < 0)
		throw std::runtime_error("socket() failed");

	setsockopt(this->_listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	addr.sin_family		 = AF_INET;
	addr.sin_port		 = htons(this->_port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_listen_sock,
			 reinterpret_cast<struct sockaddr*>(&addr),
			 sizeof(addr))
		< 0)
		throw std::runtime_error("bind() failed");

	if (listen(this->_listen_sock, MAX_EVENTS) < 0)
		throw std::runtime_error("listen() failed");

	LOG_INFO("Server listening on port " << _port);
}

int
Server::acceptClient()
{
	struct sockaddr_in clientAddr;
	socklen_t		   clientLen = sizeof(clientAddr);

	int clientFd = accept(this->_listen_sock,
						  reinterpret_cast<struct sockaddr*>(&clientAddr),
						  &clientLen);
	if (clientFd < 0)
		throw std::runtime_error("accept() failed");

	LOG_INFO("New client connected, fd=" << clientFd);
	return (clientFd);
}

int
Server::listenSockets()
{
	int clientFd = this->acceptClient();

	/* Refusal logic will be implemented with Client authentication
	When _clients.size() > MAX_EVENT()
	At the IRC Protocol Level Later :
	Wrong password,
	Banned client (if  needed in the subject),
	No more conneciton allowd, Registration Timeout */

	return (clientFd);
}

int
Server::parsePort(const char* str)
{
	char* end;
	long  n;

	for (int i = 0; str[i]; i++)
	{
		if (str[i] < '0' || str[i] > '9')
			throw std::runtime_error("Error: invalid port number '"
									 + std::string(str) + "'");
	}

	errno = 0;
	n	  = std::strtol(str, &end, 10);

	if ((errno == ERANGE) || (n <= 0) || (n > MAX_PORT))
	{
		std::ostringstream oss;
		oss << "Error: port must be between 1 and " << MAX_PORT << ", got: '"
			<< str << "'";
		throw std::runtime_error(oss.str());
	}
	return (static_cast<int>(n));
}
