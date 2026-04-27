/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:14:10 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/27 18:51:11 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>

Client::Client(int fd, std::string hostname) :
	_fd(fd),
	_hostname(hostname),
	_nickname("*"),
	_username(""),
	_realname(""),
	_inputBuffer(""),
	_passAccepted(false),
	_nickSet(false),
	_userSet(false),
	_registered(false)
{
	printClientData();
}

Client::~Client()
{
	if (this->_fd >= 0)
		close(this->_fd);
}

void
Client::printClientData() const
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
Client::getPassAccepted() const
{
	return (this->_passAccepted);
}

void
Client::setPassAccepted(bool status)
{
	this->_passAccepted = status;
}

std::string
Client::getPrefix(void) const
{
	return (this->_nickname + "!" + this->_username + "@" + this->_hostname);
}

bool
Client::isRegistered(void) const
{
	return ((this->_passAccepted && this->_nickSet && this->_userSet)
			|| this->_registered);
}

std::string
Client::getNickname(void) const
{
	return (this->_nickname);
}
