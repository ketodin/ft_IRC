/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:14:10 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/25 21:42:59 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>

Client::Client(int fd) :
	_fd(fd),
	_nickname("*"),
	_username(""),
	_realname(""),
	_inputBuffer(""),
	_isLogged(false)
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
	std::vector<std::string> messages;
	std::string::size_type	 pos;

	// guard: if buffer has no \n and exceeds 512, the client is misbehaving
	// discard everything to avoid memory bloat
	if ((this->_inputBuffer.size() > 512)
		&& (this->_inputBuffer.find('\n') == std::string::npos))
	{
		this->_inputBuffer.clear();
		return (messages);
	}

	while ((pos = this->_inputBuffer.find("\n")) != std::string::npos)
	{
		std::string msg = this->_inputBuffer.substr(0, pos);
		// strip trailing \r if present (handles both \r\n and \n)
		if (!msg.empty() && msg[msg.size() - 1] == '\r')
			msg.erase(msg.size() - 1);

		// truncate individual message at 510 (512 - \r\n)
		if (msg.size() > 510)
			msg.resize(510);

		// RFC says empty messages are silently ignored
		if (!msg.empty())
			messages.push_back(msg);

		this->_inputBuffer.erase(0, pos + 1);
	}
	return (messages);
}

int
Client::getFd() const
{
	return (this->_fd);
}

bool
Client::getIsLogged() const
{
	return (this->_isLogged);
}

void
Client::setIsLogged(bool status)
{
	this->_isLogged = status;
}
