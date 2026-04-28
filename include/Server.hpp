/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:27:59 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/28 04:28:31 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
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

		void broadcast(const std::string& msg,
					   const Client*	  except = NULL) const;

		Client*	 getClientByFd(const int fd) const;
		Client*	 getClientByNick(const std::string& name) const;
		Channel* getChannelByName(const std::string& name) const;
		Channel* getOrCreateChannel(const std::string& name);

	private:
		enum ReadStatus
		{
			READ_OK,
			READ_AGAIN,
			READ_DISCONNECT,
			READ_ERROR
		};

		const std::string	  _serverName;
		const std::string	  _creationDate;
		int					  _epoll_fd;
		int					  _listen_sock;
		int					  _port;
		const std::string	  _password;
		std::vector<Client*>  _clients;
		std::vector<Channel*> _channels;

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
