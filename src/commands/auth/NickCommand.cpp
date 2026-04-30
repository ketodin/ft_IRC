/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:50:10 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 01:51:59 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"
#include "utils.hpp"
#include <algorithm>
#include <sys/socket.h>

const std::string NickCommand::NAME = "NICK";

void
NickCommand::execute(Client& client, const std::vector<std::string>& args)
{
	if (args.empty())
	{
		ServerReply::reply(client, ServerReply::ERR_NONICKNAMEGIVEN);
		return;
	}

	try
	{
		requireWord(args, 0, "nickname");
	}
	catch (const std::exception& e)
	{
		ServerReply::reply(client, ServerReply::ERR_ERRONEUSNICKNAME, args[0]);
	}

	Server* instance = Server::getInstance();
	if (instance->getClientByNick(args[0]))
	{
		ServerReply::reply(client, ServerReply::ERR_NICKNAMEINUSE, args[0]);
		return;
	}

	if (client.getRegistered())
	{
		std::string message = ":" + client.getPrefix() + " NICK " + args[0];
		instance->broadcast(message);
	}
	if (client.firstRegistered())
	{
		client.setRegistered(true);
		instance->sendWelcomeBurst(client);
	}
	client.setNickname(args[0]);
	client.setNickSet(true);
}
