/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:27:59 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/23 21:13:07 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
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

class Server
{
	private:
		int					 _epoll_fd;
		int					 _listen_sock;
		int					 _port;
		const std::string	 _password;
		std::vector<Client*> _clients;

		void setupSocket();
		int	 acceptClient();
		void addNewClient(struct epoll_event ev);
		bool removeClient(int fd); // returns if the client we try to remove is found in the vector
		void handleEvents(struct epoll_event ev, struct epoll_event events[MAX_EVENTS], int nfds);

		ReadStatus getReadStatus(int fd, char* buffer, ssize_t& n) const;

	public:
		Server(int port, const std::string& password);
		~Server();

		static int parsePort(const char* str);
		int		   listenSockets();
		void	   start();
};

#endif
