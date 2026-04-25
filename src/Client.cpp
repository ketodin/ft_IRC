/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:14:10 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/25 17:49:18 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>

Client::Client(int fd) :
	_fd(fd),
	_nickname("*"),
	_username(""),
	_realname(""),
	_inputBuffer("")
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

void
Client::appendToBuffer(const std::string& data)
{
	this->_inputBuffer += data;
}

std::vector<std::string>
Client::extractMessages()
{
    std::vector<std::string>    messages;
    std::string::size_type      pos;

    while ((pos = this->_inputBuffer.find("\n")) != std::string::npos)
    {
        std::string msg = this->_inputBuffer.substr(0, pos);
        // strip trailing \r if present (handles both \r\n and \n)
        if (!msg.empty() && msg[msg.size() - 1] == '\r')
            msg.erase(msg.size() - 1);
        messages.push_back(msg);
        this->_inputBuffer.erase(0, pos + 1);
    }
    return messages;
}

int
Client::getFd() const
{
	return (this->_fd);
}
