/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:14:10 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/28 23:57:14 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "sys/socket.h"
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

void
Client::reply(const std::string& msg)
{
    if (send(_fd, msg.c_str(), msg.size(), 0) == -1)
        std::cerr << "send() failed for client " << _nickname << std::endl;
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
Client::getNickname() const
{
	return (this->_nickname);
}

void
Client::setNickname(const std::string& nickname)
{
	this->_nickname = nickname;
}

void
Client::setUsername(const std::string& username)
{
	this->_username = username;
}

void
Client::setRealName(const std::string& realname)
{
	this->_realname = realname;
}

void
Client::setHostName(const std::string& hostname)
{
	this->_hostname = hostname;
}

std::string
Client::getPrefix(void) const
{
	return (this->_nickname + "!" + this->_username + "@" + this->_hostname);
}

std::string
Client::getUsername() const
{
	return (this->_username);
}

std::string
Client::getHostname() const
{
	return (this->_hostname);
}

bool
Client::getRegistered(void) const
{
	return (this->_registered);
}

void
Client::setRegistered(bool registered)
{
	this->_registered = registered;
}

void
Client::setNickSet(bool nickset)
{
	this->_nickSet = nickset;
}

void
Client::setUserSet(bool userset)
{
	this->_userSet = userset;
}

bool
Client::getNickSet() const
{
	return (this->_nickSet);
}

bool
Client::getUserSet() const
{
	return (this->_userSet);
}
