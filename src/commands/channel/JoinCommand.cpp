/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 21:37:36 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/28 18:54:59 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"

const std::string JoinCommand::NAME = "JOIN";

void	JoinCommand::execute(Client &client, const std::vector<std::string>& args)
{
	if (!client.isRegistered())
	{
		std::cout << "client needs to be registered before joining a channel" << std::endl;
		return;
	}

	requireArgsNum(args, 1, 2, "JOIN <channel> <key>");
	requireChannel(args, 0, "channel");
	optionalWord(args, 1, "key");

	bool isCreator = false;
	Server *instance = Server::getInstance();
	if (!instance->getChannelByName(args[0]))
		isCreator = true;

	Channel *chan = instance->getOrCreateChannel(args[0]);
	std::string modeString = chan->getModeString();

	if (modeString == "+i")
	{
		if (!chan->isInvited(client))
		{
			// ERR_INVITEONLYCHAN (473)
			std::cout << "This channel is in invite mode only" << std::endl;
			return;
		}
	}
	else if (modeString == "+l")
	{
		if (chan->getMemberSize() >= chan->getUserLimit())
		{
			// ERR_CHANNELISFULL (471)
			std::cout << "This channel is full" << std::endl;
			return;
		}
	}
	else if (modeString == "+k")
	{
		if (args.size() < 2)
		{
			// ERR_BADCHANNELKEY (475)
			std::cout << "no channel key entered" << std::endl;
			return;
		}
		else if (args[1] != chan->getKey())
		{
			// ERR_BADCHANNELKEY (475)
			std::cout << "bad channel key" << std::endl;
			return;
		}
	}
	chan->addMember(client);
	if (isCreator)
		chan->addOperator(client);
}
