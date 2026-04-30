/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerReply.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 23:42:02 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 03:24:37 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerReply.hpp"

namespace ServerReply
{

static std::string
getReplyPrefix(const Code& code, const Client& client)
{
	return (":" + Server::getInstance()->getServerName() + " "
			+ utils::toString(static_cast<int>(code)) + " "
			+ client.getNickname() + " ");
}

static std::string
makeReply(const Code&		 code,
		  const Client&		 client,
		  const Channel*	 channel,
		  const std::string* extra)
{
	std::string rplPrefix = getReplyPrefix(code, client);
	std::string rplMsg	  = "";

	switch (code)
	{
		case (RPL_WELCOME):
			rplMsg = ":Welcome to the IRC network, " + client.getPrefix();
			break;
		case (RPL_YOURHOST):
			rplMsg = ":Your host is " + Server::getInstance()->getServerName()
					 + ", running version 1.0";
			break;
		case (RPL_CREATED):
			rplMsg = ":This server was created "
					 + Server::getInstance()->getCreationDate();
			break;
		case (RPL_MYINFO):
			rplMsg = Server::getInstance()->getServerName()
					 + " 1.0 :Available channel modes: 1.0 io itkol";
			break;
		case (ERR_NOMOTD):
			rplMsg = ":MOTD File is missing";
			break;

		case (RPL_CHANNELMODEIS):
			rplMsg = channel->getName() + " " + channel->getModeString();
			break;
		case (ERR_NOSUCHCHANNEL):
			rplMsg = *extra + " :No such channel";
			break;
		case (ERR_CANNOTSENDTOCHAN):
			rplMsg = *extra + " :Cannot send to channel";
			break;
		case (RPL_NOTOPIC):
			rplMsg = channel->getName() + " :No topic is set";
			break;
		case (RPL_TOPIC):
			rplMsg = channel->getName() + " :" + channel->getTopic();
			break;
		case (RPL_INVITING):
			rplMsg = *extra + " " + channel->getName();
			break;
		case (RPL_NAMREPLY):
			rplMsg = "= " + channel->getName() + " :" + *extra;
			break;
		case (RPL_ENDOFNAMES):
			rplMsg = channel->getName() + " :End of /NAMES list";
			break;

		case (ERR_NOTEXTTOSEND):
			rplMsg = ":No text to send";
			break;
		case (ERR_NONICKNAMEGIVEN):
			rplMsg = ":No nickname given";
			break;
		case (ERR_NOTREGISTERED):
			rplMsg = ":You have not registered";
			break;
		case (ERR_ALREADYREGISTERED):
			rplMsg = ":You may not reregister";
			break;
		case (ERR_PASSWDMISMATCH):
			rplMsg = ":Password incorrect";
			break;

		case (ERR_UNKNOWNCOMMAND):
			rplMsg = *extra + " :Unknown command";
			break;
		case (ERR_NEEDMOREPARAMS):
			rplMsg = *extra + " :Not enough parameters";
			break;
		case (ERR_NOSUCHNICK):
			rplMsg = *extra + " :No such nick/channel";
			break;
		case (ERR_NORECIPIENT):
			rplMsg = ":No recipient given (PRIVMSG)";
			break;
		case (ERR_ERRONEUSNICKNAME):
			rplMsg = *extra + " :Erroneous nickname";
			break;
		case (ERR_NICKNAMEINUSE):
			rplMsg = *extra + " :Nickname is already in use";
			break;
		case (ERR_NOTONCHANNEL):
			rplMsg = channel->getName() + " :You're not on that channel";
			break;
		case (ERR_CHANOPRIVSNEEDED):
			rplMsg = channel->getName() + " :You're not channel operator";
			break;
		case (ERR_CHANNELISFULL):
			rplMsg = channel->getName() + " :Cannot join channel (+l)";
			break;
		case (ERR_INVITEONLYCHAN):
			rplMsg = channel->getName() + " :Cannot join channel (+i)";
			break;
		case (ERR_BADCHANNELKEY):
			rplMsg = channel->getName() + " :Cannot join channel (+k)";
			break;
		case (ERR_UNKNOWNMODE):
			rplMsg = *extra + " :is unknown mode char to me";
			break;

		case (ERR_USERNOTINCHANNEL):
			rplMsg = *extra + " " + channel->getName()
					 + " :They aren't on that channel";
			break;
		case (ERR_USERONCHANNEL):
			rplMsg =
				*extra + " " + channel->getName() + " :is already on channel";
			break;
		case (ERR_NOORIGIN):
			rplMsg = ":No origin specified";
			break;

		default:
			rplMsg = ":Unknown reply";
			break;
	}
	return (rplPrefix + rplMsg);
}

void
reply(const Client& client, const Code& code)
{
	client.reply(makeReply(code, client, NULL, NULL));
}

void
reply(const Client& client, const Code& code, const std::string& extra)
{
	client.reply(makeReply(code, client, NULL, &extra));
}

void
reply(const Client& client, const Channel& channel, const Code& code)
{
	client.reply(makeReply(code, client, &channel, NULL));
}

void
reply(const Client&		 client,
	  const Channel&	 channel,
	  const Code&		 code,
	  const std::string& extra)
{
	client.reply(makeReply(code, client, &channel, &extra));
}

}
