/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:27:59 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/24 13:44:54 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Utils.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <vector>

#define MAX_EVENTS 50

#define MAX_PORT 65535

#define BUFFER_SIZE 1024

#ifndef DEBUG
#	define DEBUG 0
#endif

#if DEBUG
#	define LOG_INFO(msg) std::cout << "[DEBUG] " << msg << std::endl
#else
#	define LOG_INFO(msg)                                                      \
		do                                                                     \
		{                                                                      \
		} while (0)
#endif

enum ReadStatus
{
	READ_OK,
	READ_AGAIN,		 // EAGAIN / EWOULDBLOCK — no data yet
	READ_DISCONNECT, // n == 0 — client closed connection cleanly
	READ_ERROR		 // n < 0 with a real error
};

struct HasFd
{
		int _fd;

		explicit HasFd(int fd) :
			_fd(fd)
		{
		}

		bool
		operator()(const Client* client) const
		{
			return client->getFd() == _fd;
		}
};

class Server
{
	private:
		int					 _epoll_fd;
		int					 _listen_sock;
		int					 _port;
		const std::string	 _password;
		std::vector<Client*> _clients;

		// socket handling.
		void setupSocket();
		int	 acceptClient();
		int	 listenSockets();

		// client handling
		void addNewClient();
		bool removeClient(int fd); // returns if the client we try to remove is
								   // found in the vector
		void handleEvents(struct epoll_event events[MAX_EVENTS], int nfds);
		void setNonBlocking(int fd);

		ReadStatus getReadStatus(int fd, char* buffer, ssize_t& n) const;

	public:
		Server(int port, const std::string& password);
		~Server();

		static int parsePort(const char* str);
		void	   start();

		class ServerException : public std::runtime_error
		{
			public:
				explicit ServerException(const std::string& msg) :
					std::runtime_error("[Server Error]: " + msg)
				{
				}

				virtual ~ServerException() throw()
				{
				}
		};

		class EpollCreateException : public ServerException
		{
			public:
				explicit EpollCreateException() :
					ServerException("epoll_create() failed: "
									+ std::string(strerror(errno)))
				{
				}
		};

		class FcntlException : public ServerException
		{
			public:
				explicit FcntlException(const std::string& flag) :
					ServerException("fcntl() failed with flag: " + flag + ": "
									+ std::string(strerror(errno)))
				{
				}
		};

		class EpollCtlException : public ServerException
		{
			public:
				explicit EpollCtlException(const std::string& op) :
					ServerException("epoll_ctl() failed on operation: " + op
									+ ": " + std::string(strerror(errno)))
				{
				}
		};

		class EpollWaitException : public ServerException
		{
			public:
				explicit EpollWaitException() :
					ServerException("epoll_wait() failed: "
									+ std::string(strerror(errno)))
				{
				}
		};

		class CreateSocketException : public ServerException
		{
			public:
				explicit CreateSocketException() :
					ServerException("socket() failed: "
									+ std::string(strerror(errno)))
				{
				}
		};

		class SocketOptException : public ServerException
		{
			public:
				explicit SocketOptException(const std::string& opt) :
					ServerException("setsockopt() failed on option: " + opt
									+ ": " + std::string(strerror(errno)))
				{
				}
		};

		class BindException : public ServerException
		{
			public:
				explicit BindException(int port) :
					ServerException("bind() failed on port: " + toString(port)
									+ ": " + std::string(strerror(errno)))
				{
				}
		};

		class ListenException : public ServerException
		{
			public:
				explicit ListenException(int port) :
					ServerException("listen() failed on port: " + toString(port)
									+ ": " + std::string(strerror(errno)))
				{
				}
		};

		class AcceptException : public ServerException
		{
			public:
				explicit AcceptException() :
					ServerException("accept() failed: "
									+ std::string(strerror(errno)))
				{
				}
		};

		class PortNumberException : public ServerException
		{
			public:
				explicit PortNumberException(const std::string& port) :
					ServerException("invalid port number: '" + port + "'")
				{
				}
		};

		class InvalidPortRangeException : public ServerException
		{
			public:
				explicit InvalidPortRangeException(const std::string& port) :
					ServerException("port must be between 1 and "
									+ toString(MAX_PORT) + ", got: '" + port
									+ "'")
				{
				}
		};
};

#endif
