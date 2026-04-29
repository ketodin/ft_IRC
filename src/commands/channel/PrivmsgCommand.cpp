/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 21:49:24 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 00:37:45 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivmsgCommand.hpp"
#include "ServerReply.hpp"

const std::string PrivmsgCommand::NAME = "PRIVMSG";

void
PrivmsgCommand::execute(
	Client& client, // cppcheck-suppress constParameterReference
	const std::vector<std::string>& args)
{
	if (!client.getRegistered())
	{
		ServerReply::reply(client, ServerReply::ERR_NOTREGISTERED);
		return;
	}

	try
	{
		requireArgsNum(args, 2, "PRIVMSG <target> :<message>");
	}
	catch (const std::exception& e)
	{
		ServerReply::reply(client, ServerReply::ERR_NEEDMOREPARAMS, "PRIVMSG");
		std::cerr << e.what() << '\n';
		return;
	}

	if (args[0].empty())
	{
		ServerReply::reply(client, ServerReply::ERR_NORECIPIENT);
		return;
	}

	if (args[1].empty())
	{
		ServerReply::reply(client, ServerReply::ERR_NOTEXTTOSEND);
		return;
	}

	const std::string& target = args[0];
	const std::string& msg	  = args[1];
	const std::string  prefix = ":" + client.getPrefix();

	const Server* instance = Server::getInstance();
	if (target[0] == '#')
	{
		Channel* chan = instance->getChannelByName(target);

		if (!chan)
		{
			ServerReply::reply(client, ServerReply::ERR_NOSUCHCHANNEL, target);
			return;
		}

		if (!chan->isMember(client))
		{
			ServerReply::reply(
				client, ServerReply::ERR_CANNOTSENDTOCHAN, target);
			return;
		}
		chan->broadcast(prefix + " PRIVMSG " + target + " :" + msg + "\r\n",
						&client);
	}
	else
	{
		const Client* dest = instance->getClientByNick(target);

		if (!dest)
		{
			ServerReply::reply(client, ServerReply::ERR_NOSUCHNICK, target);
			return;
		}

		dest->reply(prefix + " PRIVMSG " + target + " :" + msg + "\r\n");
	}
}
