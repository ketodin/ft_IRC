/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 21:37:36 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/28 22:27:55 by lcalero          ###   ########.fr       */
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
		return; // ERR_NOTREGISTERED 451

	requireArgsNum(args, 1, 2, "JOIN <channel> <key>");
	requireChannel(args, 0, "channel");
	optionalWord(args, 1, "key");

	Server*	 instance  = Server::getInstance();
	Channel* chan	   = instance->getChannelByName(args[0]);
	bool	 isCreator = (chan == NULL);

	if (!isCreator && !checkAccess(client, *chan, args))
		return;

	if (isCreator)
		chan = instance->getOrCreateChannel(args[0]);

	chan->addMember(client);
}

bool
JoinCommand::checkAccess(const Client&					 client,
						 const Channel&					 chan,
						 const std::vector<std::string>& args)
{
	if (chan.hasMode('i') && !chan.isInvited(client))
	{
		// ERR_INVITEONLYCHAN 473
		return (false);
	}
	if (chan.hasMode('k'))
	{
		if (args.size() < 2 || !chan.isValidKey(args[1]))
		{
			// ERR_BADCHANNELKEY 475
			return (false);
		}
	}
	if (chan.hasMode('l') && chan.getMemberSize() >= chan.getUserLimit())
	{
		// ERR_CHANNELISFULL 471
		return (false);
	}
	return (true);
}
