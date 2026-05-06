/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 21:37:36 by jaubry--          #+#    #+#             */
/*   Updated: 2026/05/05 23:52:08 by jaubry--         ###   ########.fr       */
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
	try
	{
		requireChannel(args, 0, "channel");
	}
	catch (const std::exception& e)
	{
		ServerReply::reply(client, ServerReply::ERR_BADCHANMASK, args[0]);
		return;
	}
	optionalWord(args, 1, "key");

	Server*	 instance = Server::getInstance();
	Channel* chan	  = instance->getChannelByName(args[0]);

	if (chan && !checkAccess(client, *chan, args))
		return;

	chan = instance->getOrCreateChannel(args[0]);
	chan->addMember(client);
	sendJoinBurst(client, *chan);
}

bool
JoinCommand::checkAccess(const Client&					 client,
						 const Channel&					 chan,
						 const std::vector<std::string>& args)
{
	if (chan.getInviteMode() && !chan.isInvited(client))
	{
		ServerReply::reply(client, chan, ServerReply::ERR_INVITEONLYCHAN);
		return (false);
	}
	if (!chan.isValidKey(""))
	{
		if ((args.size() < 2) || !chan.isValidKey(args[1]))
		{
			ServerReply::reply(client, chan, ServerReply::ERR_BADCHANNELKEY);
			return (false);
		}
	}
	if ((chan.getUserLimit() != 0)
		&& (chan.getMemberSize() >= chan.getUserLimit()))
	{
		ServerReply::reply(client, chan, ServerReply::ERR_CHANNELISFULL);
		return (false);
	}
	return (true);
}

void
JoinCommand::sendJoinBurst(const Client& client, Channel& chan)
{
	const std::string name = chan.getName();

	// 1. JOIN broadcast to all members including joining client
	chan.broadcast(":" + client.getPrefix() + " JOIN :" + name);

	// 2. RPL_TOPIC 332 only if topic is set
	if (!chan.getTopic().empty())
		ServerReply::reply(client, chan, ServerReply::RPL_TOPIC);

	// 3. RPL_NAMREPLY 353
	ServerReply::reply(
		client, chan, ServerReply::RPL_NAMREPLY, chan.buildNamesReply());

	// 4. RPL_ENDOFNAMES 366
	ServerReply::reply(client, chan, ServerReply::RPL_ENDOFNAMES);
}
