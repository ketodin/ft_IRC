/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerExceptions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 22:31:11 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/24 23:03:29 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

HasFd::HasFd(int fd) :
	_fd(fd)
{
}

bool
HasFd::operator()(const Client* client) const
{
	return client->getFd() == _fd;
}

// ServerException

Server::ServerException::ServerException(const std::string& msg) :
	std::runtime_error("[Server Error]: " + msg)
{
}

Server::ServerException::~ServerException() throw()
{
}

// EpollCreateException

Server::EpollCreateException::EpollCreateException() :
	ServerException("epoll_create() failed: " + std::string(strerror(errno)))
{
}

// FcntlException

Server::FcntlException::FcntlException(const std::string& flag) :
	ServerException("fcntl() failed with flag: " + flag + ": "
					+ std::string(strerror(errno)))
{
}

// EpollCtlException

Server::EpollCtlException::EpollCtlException(const std::string& op) :
	ServerException("epoll_ctl() failed on operation: " + op + ": "
					+ std::string(strerror(errno)))
{
}

// EpollWaitException

Server::EpollWaitException::EpollWaitException() :
	ServerException("epoll_wait() failed: " + std::string(strerror(errno)))
{
}

// CreateSocketException

Server::CreateSocketException::CreateSocketException() :
	ServerException("socket() failed: " + std::string(strerror(errno)))
{
}

// SocketOptException

Server::SocketOptException::SocketOptException(const std::string& opt) :
	ServerException("setsockopt() failed on option: " + opt + ": "
					+ std::string(strerror(errno)))
{
}

// BindException

Server::BindException::BindException(int port) :
	ServerException("bind() failed on port: " + toString(port) + ": "
					+ std::string(strerror(errno)))
{
}

// ListenException

Server::ListenException::ListenException(int port) :
	ServerException("listen() failed on port: " + toString(port) + ": "
					+ std::string(strerror(errno)))
{
}

// AcceptException

Server::AcceptException::AcceptException() :
	ServerException("accept() failed: " + std::string(strerror(errno)))
{
}

// PortNumberException

Server::PortNumberException::PortNumberException(const std::string& port) :
	ServerException("invalid port number: '" + port + "'")
{
}

// InvalidPortRangeException

Server::InvalidPortRangeException::InvalidPortRangeException(
	const std::string& port) :
	ServerException("port must be between 1 and " + toString(MAX_PORT)
					+ ", got: '" + port + "'")
{
}