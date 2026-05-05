/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 22:04:33 by jaubry--          #+#    #+#             */
/*   Updated: 2026/05/05 18:24:31 by jaubry--         ###   ########.fr       */
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

	const Client* target = Server::getInstance()->getClientByNick(args[0]);
	Channel*	  chan	 = Server::getInstance()->getChannelByName(args[1]);

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
	if (!checkInvitePossible(client, *target, *chan, args))
		return;
	ServerReply::reply(client, *chan, ServerReply::RPL_INVITING, args[0]);
	target->reply(":" + client.getPrefix() + " INVITE "
				  + target->getNickname() + " " + chan->getName());
	chan->addInvite(*target);
}

bool
InviteCommand::checkInvitePossible(const Client&				   client,
								   const Client&				   target,
								   const Channel&				   chan,
								   const std::vector<std::string>& args)
{
	if (!chan.isMember(client))
	{
		ServerReply::reply(client, chan, ServerReply::ERR_NOTONCHANNEL);
		return (false);
	}
	else if (chan.getInviteMode() && !chan.isOperator(client))
	{
		ServerReply::reply(client, chan, ServerReply::ERR_CHANOPRIVSNEEDED);
		return (false);
	}
	else if (chan.isMember(target))
	{
		ServerReply::reply(
			client, chan, ServerReply::ERR_USERONCHANNEL, args[0]);
		return (false);
	}
	return (true);
}
