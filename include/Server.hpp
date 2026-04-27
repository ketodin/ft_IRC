/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:27:59 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/27 18:29:08 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "utils.hpp"
#include <arpa/inet.h>
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

		static Server* getInstance(void);
		static void	   init(int port, const std::string& password);
		static void	   destroy(void);

		static int parsePort(const char* str);
		void	   start(void);
		bool	   isPasswordValid(const std::string& password);

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

		static Server* _instance;

		Server(int port, const std::string& password);
		~Server(void);
		Server(const Server& other);
		Server& operator=(const Server& other);

		void setupSocket(void);
		void acceptClient(int& clientFd, std::string& clientHostname);

		void addNewClient(void);
		bool removeClient(int fd);
		void handleEvents(struct epoll_event events[MAX_EVENTS], int nfds);
		static void setNonBlocking(int fd);

		static ReadStatus getReadStatus(int fd, char* buffer, ssize_t& n);

#include "ServerException.inl"
};

#endif // SERVER_HPP
