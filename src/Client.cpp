/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:14:10 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/23 17:24:50 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>

Client::Client(int fd) :
	_fd(fd),
	_nickname("*"),
	_username(""),
	_realname("")
{
	printClientData();
}

Client::~Client()
{
	if (_fd >= 0)
		close(_fd);
}

void
Client::printClientData()
{
	std::cout << _fd << std::endl;
	std::cout << _nickname << std::endl;
	std::cout << _username << std::endl;
	std::cout << _realname << std::endl;
}
