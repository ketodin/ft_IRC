/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 21:01:11 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/29 01:16:10 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sys/socket.h>

Channel::Channel(const std::string& name) :
	_name(name),
	_topic(""),
	_inviteOnly(false),
	_topicRestricted(false),
	_key(""),
	_userLimit(0)
{
}

Channel::~Channel(void)
{
}

void
Channel::addMember(const Client& client)
{
	if (!this->isMember(client))
	{
		if ((this->_userLimit == 0)
			|| (this->_userLimit > this->_members.size()))
		{
			if (this->_members.size() == 0)
				this->addOperator(client);
			this->_members.push_back(const_cast<Client*>(&client));
		}
		else
			std::cout << "no more place inside channel\n";
	}
}

void
Channel::removeMember(const Client& client)
{
	std::vector<Client*>::iterator it =
		std::find(this->_members.begin(), this->_members.end(), &client);

	if (it != this->_members.end())
		this->_members.erase(it);
}

bool
Channel::isMember(const Client& client) const
{
	return (std::find(this->_members.begin(), this->_members.end(), &client)
			!= this->_members.end());
}

void
Channel::addOperator(const Client& client)
{
	if (!this->isOperator(client))
	{
		this->_operators.push_back(const_cast<Client*>(&client));
	}
}

void
Channel::removeOperator(const Client& client)
{
	std::vector<Client*>::iterator it =
		std::find(this->_operators.begin(), this->_operators.end(), &client);

	if (it != this->_operators.end())
		this->_operators.erase(it);
}

bool
Channel::isOperator(const Client& client) const
{
	return (std::find(this->_operators.begin(), this->_operators.end(), &client)
			!= this->_operators.end());
}

void
Channel::addInvite(const Client& client)
{
	if (!this->isInvited(client))
	{
		this->_invites.push_back(const_cast<Client*>(&client));
	}
}

bool
Channel::isInvited(const Client& client) const
{
	return (std::find(this->_invites.begin(), this->_invites.end(), &client)
			!= this->_invites.end());
}

void
Channel::broadcast(const std::string& msg, const Client* except)
{
	std::string finalMessage = msg + "\r\n";
	for (std::vector<Client*>::const_iterator it = _members.begin();
		 it != _members.end();
		 ++it)
	{
		const Client* currentClient = *it;

		if (!except)
			this->sendMsg(*currentClient, finalMessage);
		else if (currentClient->getFd() != except->getFd())
			this->sendMsg(*currentClient, finalMessage);
	}
}

void
Channel::sendMsg(const Client& client, const std::string& msg)
{
	ssize_t sent = send(client.getFd(), msg.c_str(), msg.size(), 0);

	if (sent == -1)
		std::cout << "send(): failed" << std::endl;
	else if (sent < static_cast<ssize_t>(msg.size()))
		std::cout << "send(): message partially sent" << std::endl;
}

std::string
Channel::getModeString(void) const
{
	std::string flags  = "+";
	std::string params = "";

	if (this->_inviteOnly)
		flags += "i";
	if (this->_topicRestricted)
		flags += "t";
	if (!this->_key.empty())
	{
		flags += "k";
		params += " " + this->_key;
	}
	if (this->_userLimit > 0)
	{
		flags += "l";
		std::ostringstream oss;
		oss << this->_userLimit;
		params += " " + oss.str();
	}

	return (flags + params);
}

std::string
Channel::buildNamesReply(void) const
{
	std::string names = "";

	for (std::size_t i = 0; i < this->_members.size(); i++)
	{
		if (i > 0)
			names += " ";
		if (this->isOperator(*this->_members[i]))
			names += "@";
		names += this->_members[i]->getNickname();
	}

	return (names);
}

std::string
Channel::getName(void) const
{
	return (this->_name);
}

std::string
Channel::getTopic(void) const
{
	return (this->_topic);
}
