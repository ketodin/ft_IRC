/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:27:59 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/25 18:32:05 by ekeisler         ###   ########.fr       */
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

class Server
{
	public:
		static const int MAX_EVENTS	 = 50;
		static const int MAX_PORT	 = 65535;
		static const int BUFFER_SIZE = 512;

		Server(int port, const std::string& password);
		~Server();

		static int parsePort(const char* str);
		void	   start(void);

	private:
		enum ReadStatus
		{
			READ_OK,
			READ_AGAIN,
			READ_DISCONNECT,
			READ_ERROR
		};

		int					 _epoll_fd;
		int					 _listen_sock;
		int					 _port;
		const std::string	 _password;
		std::vector<Client*> _clients;

		void setupSocket(void);
		int	 acceptClient(void);
		int	 listenSockets(void);

		void addNewClient(void);
		bool removeClient(int fd);
		void handleEvents(struct epoll_event events[MAX_EVENTS], int nfds);
		static void setNonBlocking(int fd);

		static ReadStatus getReadStatus(int fd, char* buffer, ssize_t& n);

#include "ServerException.inl"
};

#endif // SERVER_HPP
