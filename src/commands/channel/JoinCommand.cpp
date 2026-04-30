/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 21:37:36 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 01:43:05 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"

const std::string JoinCommand::NAME = "JOIN";

void
JoinCommand::execute(
	Client& client, // cppcheck-suppress constParameterReference
	const std::vector<std::string>& args)
{
	if (!client.getRegistered())
	{
		ServerReply::reply(client, ServerReply::ERR_NOTREGISTERED);
		return;
	}

	requireArgsNum(args, 1, 2, "JOIN <channel> <key>");
	requireChannel(args, 0, "channel");
	optionalWord(args, 1, "key");

	Server*	 instance = Server::getInstance();
	Channel* chan	  = instance->getChannelByName(args[0]);

	if (chan && !checkAccess(client, *chan, args))
		return;

	chan = instance->getOrCreateChannel(args[0]);
	chan->addMember(client);
	instance->sendJoinBurst(client, *chan);
}

bool
JoinCommand::checkAccess(const Client&					 client,
						 const Channel&					 chan,
						 const std::vector<std::string>& args)
{
	if (chan.hasMode('i') && !chan.isInvited(client))
	{
		ServerReply::reply(client, chan, ServerReply::ERR_INVITEONLYCHAN);
		return (false);
	}
	if (chan.hasMode('k'))
	{
		if (args.size() < 2 || !chan.isValidKey(args[1]))
		{
			ServerReply::reply(client, chan, ServerReply::ERR_BADCHANNELKEY);
			return (false);
		}
	}
	if (chan.hasMode('l') && chan.getMemberSize() >= chan.getUserLimit())
	{
		ServerReply::reply(client, chan, ServerReply::ERR_CHANNELISFULL);
		return (false);
	}
	return (true);
}
