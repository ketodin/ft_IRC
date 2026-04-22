/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:52:35 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/22 21:00:24 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
}

Server::~Server()
{
}
