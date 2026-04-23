/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:14:10 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/23 19:28:18 by lcalero          ###   ########.fr       */
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
	if (this->_fd >= 0)
		close(this->_fd);
}

void
Client::printClientData()
{
	std::cout << this->_fd << std::endl;
	std::cout << this->_nickname << std::endl;
	std::cout << this->_username << std::endl;
	std::cout << this->_realname << std::endl;
}
