/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 22:08:37 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 02:19:44 by lcalero          ###   ########.fr       */
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

	if (!checkChannelAccess(client, chan, args))
		return;

	if (args.size() == 1)
	{
		if (chan->getTopic().empty())
			ServerReply::reply(client, *chan, ServerReply::RPL_NOTOPIC);
		else
			ServerReply::reply(client, *chan, ServerReply::RPL_TOPIC);
	}

	chan->setTopic(args[1]);

	const std::string msg = "TOPIC " + chan->getName() + " " + chan->getTopic();
	chan->broadcast(msg);
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
	else if (channel->hasMode('t') && !channel->isOperator(client))
	{
		ServerReply::reply(client, *channel, ServerReply::ERR_CHANOPRIVSNEEDED);
		return (false);
	}
	return (true);
}
