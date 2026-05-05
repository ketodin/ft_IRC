/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 22:08:37 by jaubry--          #+#    #+#             */
/*   Updated: 2026/05/05 18:18:12 by jaubry--         ###   ########.fr       */
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
	requireArgsNum(args, 1, 2, "TOPIC <channel> <topic>");
	requireChannel(args, 0, "channel");
	optionalParam(args, 1, "topic");

	const Server* instance = Server::getInstance();
	Channel*	  chan	   = instance->getChannelByName(args[0]);

	if (!checkChannelAccess(client, chan, args))
		return;

	if (args.size() == 1)
	{
		if (chan->getTopic().empty())
			ServerReply::reply(client, *chan, ServerReply::RPL_NOTOPIC);
		else
			ServerReply::reply(client, *chan, ServerReply::RPL_TOPIC);
		return;
	}

	chan->setTopic(args[1]);

	const std::string msg = "TOPIC " + chan->getName() + " :" + chan->getTopic();
	chan->broadcast(client, msg);
}

bool
TopicCommand::checkChannelAccess(const Client&					 client,
								 const Channel*					 channel,
								 const std::vector<std::string>& args)
{
	if (!channel)
	{
		ServerReply::reply(client, ServerReply::ERR_NOSUCHCHANNEL, args[0]);
		return (false);
	}
	else if (!channel->isMember(client))
	{
		ServerReply::reply(client, *channel, ServerReply::ERR_NOTONCHANNEL);
		return (false);
	}
	else if (channel->getTopicMode() && !channel->isOperator(client))
	{
		ServerReply::reply(client, *channel, ServerReply::ERR_CHANOPRIVSNEEDED);
		return (false);
	}
	return (true);
}
