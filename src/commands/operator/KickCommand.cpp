/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 22:07:25 by jaubry--          #+#    #+#             */
/*   Updated: 2026/05/05 18:55:36 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KickCommand.hpp"

const std::string KickCommand::NAME = "KICK";

void
KickCommand::execute(
	Client& client, // cppcheck-suppress constParameterReference
	const std::vector<std::string>& args)
{
	requireArgsNum(args, 2, 3, "KICK <channel> <nick>");
	requireChannel(args, 0, "channel");
	requireWord(args, 1, "nickname");
	optionalParam(args, 2, "comment");

	Channel*	  chan	 = Server::getInstance()->getChannelByName(args[0]);
	const Client* target = Server::getInstance()->getClientByNick(args[1]);

	if (!chan)
	{
		ServerReply::reply(client, ServerReply::ERR_NOSUCHCHANNEL, args[0]);
		return;
	}
	if (!target)
		return;
	if (target->getNickname() == client.getNickname())
		return;
	if (!isValidKick(client, *target, *chan))
		return;

	std::string comment =
		(args.size() == 3 && !args[2].empty()) ? args[2] : "bad behavior";

	chan->broadcast(client,
					"KICK " + chan->getName() + " " + target->getNickname()
						+ " :" + comment);
	chan->removeMember(*target);
}

bool
KickCommand::isValidKick(const Client&	client,
						 const Client&	target,
						 const Channel& chan)
{
	if (!chan.isMember(client))
	{
		ServerReply::reply(client, chan, ServerReply::ERR_NOTONCHANNEL);
		return (false);
	}
	if (!chan.isOperator(client))
	{
		ServerReply::reply(client, chan, ServerReply::ERR_CHANOPRIVSNEEDED);
		return (false);
	}
	if (!chan.isMember(target))
	{
		ServerReply::reply(client,
						   chan,
						   ServerReply::ERR_USERNOTINCHANNEL,
						   target.getNickname());
		return (false);
	}
	return (true);
}
