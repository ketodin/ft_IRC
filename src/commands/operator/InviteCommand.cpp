/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 22:04:33 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 21:52:09 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCommand.hpp"

const std::string InviteCommand::NAME = "INVITE";

void
InviteCommand::execute(
	Client& client, // cppcheck-suppress constParameterReference
	const std::vector<std::string>& args)
{
	requireArgsNum(args, 2, "INVITE <nickname> <channel>");
	requireWord(args, 0, "nickname");
	requireChannel(args, 1, "channel");

	const Client*  target = Server::getInstance()->getClientByNick(args[0]);
	const Channel* chan	  = Server::getInstance()->getChannelByName(args[1]);

	if (!target)
	{
		ServerReply::reply(client, ServerReply::ERR_NOSUCHNICK, args[0]);
		return;
	}
	else if (!chan)
	{
		ServerReply::reply(client, ServerReply::ERR_NOSUCHCHANNEL, args[1]);
		return;
	}
	else if (!chan->isMember(client))
	{
		ServerReply::reply(client, *chan, ServerReply::ERR_NOTONCHANNEL);
		return;
	}
	else if (chan->getInviteMode() && !chan->isOperator(client))
	{
		ServerReply::reply(client, *chan, ServerReply::ERR_CHANOPRIVSNEEDED);
		return;
	}
	else if (chan->isMember(*target))
	{
		ServerReply::reply(
			client, *chan, ServerReply::ERR_USERONCHANNEL, args[0]);
		return;
	}
	ServerReply::reply(client, *chan, ServerReply::RPL_INVITING);
	target->reply("INVITE " + args[0] + " " + args[1]);
}
