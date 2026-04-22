/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:27:59 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/22 21:00:13 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <vector>

#define MAX_EVENTS 50

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
	private:
		int _epoll_fd;
		int _listen_sock;
		int _port;
		const std::string _password;

	public:
		Server(int port, const std::string& password);
		~Server();

		// returns client's fd
		int listen_sockets();
};

#endif
