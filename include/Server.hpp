/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:27:59 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/25 03:14:52 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "utils.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <vector>

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
	READ_AGAIN,
	READ_DISCONNECT,
	READ_ERROR
};

class Server
{
	public:
		static const int MAX_EVENTS	 = 50;
		static const int MAX_PORT	 = 65535;
		static const int BUFFER_SIZE = 1024;

		Server(int port, const std::string& password);
		~Server();

		static int parsePort(const char* str);
		void	   start();

		class ServerException : public std::runtime_error
		{
			public:
				explicit ServerException(const std::string& msg);
				virtual ~ServerException() throw();
		};

		class EpollCreateException : public ServerException
		{
			public:
				explicit EpollCreateException();
		};

		class FcntlException : public ServerException
		{
			public:
				explicit FcntlException(const std::string& flag);
		};

		class EpollCtlException : public ServerException
		{
			public:
				explicit EpollCtlException(const std::string& op);
		};

		class EpollWaitException : public ServerException
		{
			public:
				explicit EpollWaitException();
		};

		class CreateSocketException : public ServerException
		{
			public:
				explicit CreateSocketException();
		};

		class SocketOptException : public ServerException
		{
			public:
				explicit SocketOptException(const std::string& opt);
		};

		class BindException : public ServerException
		{
			public:
				explicit BindException(int port);
		};

		class ListenException : public ServerException
		{
			public:
				explicit ListenException(int port);
		};

		class AcceptException : public ServerException
		{
			public:
				explicit AcceptException();
		};

		class PortNumberException : public ServerException
		{
			public:
				explicit PortNumberException(const std::string& port);
		};

		class InvalidPortRangeException : public ServerException
		{
			public:
				explicit InvalidPortRangeException(const std::string& port);
		};

	private:
		int					 _epoll_fd;
		int					 _listen_sock;
		int					 _port;
		const std::string	 _password;
		std::vector<Client*> _clients;

		void setupSocket();
		int	 acceptClient();
		int	 listenSockets();

		void addNewClient();
		bool removeClient(int fd);
		void handleEvents(struct epoll_event events[MAX_EVENTS], int nfds);
		static void setNonBlocking(int fd);

		static ReadStatus getReadStatus(int fd, char* buffer, ssize_t& n);
};

#endif
