/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 22:08:37 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 00:52:09 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"
#include "ServerReply.hpp"

const std::string TopicCommand::NAME = "TOPIC";

void
TopicCommand::execute(
	Client& client, // cppcheck-suppress constParameterReference
	const std::vector<std::string>& args)
{
	if (args.empty())
		ServerReply::reply(client, ServerReply::ERR_NEEDMOREPARAMS);

	const Server* instance = Server::getInstance();
	Channel*	  chan	   = instance->getChannelByName(args[0]);

	if (!chan)
	{
		ServerReply::reply(client, *chan, ServerReply::ERR_NOSUCHCHANNEL);
		return;
	}
	else if (!chan->isMember(client))
	{
		ServerReply::reply(client, *chan, ServerReply::ERR_NOTONCHANNEL);
		return;
	}
	else if (chan->hasMode('t') && !chan->isOperator(client))
	{
		ServerReply::reply(client, *chan, ServerReply::ERR_CHANOPRIVSNEEDED);
		return;
	}

	if (args.size() == 1)
	{
		ServerReply::reply(client, *chan, ServerReply::RPL_NOTOPIC);
		return;
	}
	chan->setTopic(args[1]);

	const std::string msg = "TOPIC " + chan->getName() + " " + chan->getTopic();
	chan->broadcast(msg);
}
