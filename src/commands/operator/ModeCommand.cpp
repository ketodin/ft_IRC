/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 22:12:43 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 05:20:05 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"
#include "ServerReply.hpp"

const std::string ModeCommand::NAME = "MODE";

void
ModeCommand::execute(Client& client, const std::vector<std::string>& args)
{
	requireArgsNum(args, 1, 3, "MODE #<channel> <mode>");
	requireChannel(args, 0, "channel name");
	Channel* chan = Server::getInstance()->getChannelByName(args[0]);
	if (!chan)
	{
		ServerReply::reply(client, ServerReply::ERR_NOSUCHCHANNEL, args[0]);
		return ;
	}

	if (args.size() > 1)
	{
		if (chan->isOperator(client))
		{
			requireMode(args, 1, "channel mode");
			getHandlers()[args[1]](client, *chan, args);
		}
		else
			ServerReply::reply(client, *chan, ServerReply::ERR_CHANOPRIVSNEEDED);
	}
	else
		ServerReply::reply(client, *chan, ServerReply::RPL_CHANNELMODEIS);
}

std::map<std::string, ModeHandler>&
ModeCommand::getHandlers(void)
{
	static std::map<std::string, ModeHandler> handlers;
	if (handlers.empty())
	{
		handlers["+i"] = &ModeCommand::modeInvite;
		handlers["-i"] = &ModeCommand::modeInvite;
		handlers["+t"] = &ModeCommand::modeTopic;
		handlers["-t"] = &ModeCommand::modeTopic;
		handlers["+k"] = &ModeCommand::modeKeyOn;
		handlers["-k"] = &ModeCommand::modeKeyOff;
		handlers["+o"] = &ModeCommand::modeOpOn;
		handlers["-o"] = &ModeCommand::modeOpOff;
		handlers["+l"] = &ModeCommand::modeLimitOn;
		handlers["-l"] = &ModeCommand::modeLimitOff;
	}
	return (handlers);
}

static void
printMode(Channel& channel,
		  const std::string& mode,
		  const std::string& action,
		  const std::string& param = "")
{
	std::cout << "MODE " << channel.getName() << " " << mode << " | " << action
			  << (param.empty() ? "" : " [" + param + "]") << std::endl;
}

void
ModeCommand::modeInvite(Client& client, Channel& chan,
						  const std::vector<std::string>& args)
{
	(void)client;
	requireArgsNum(args, 2, "MODE #<channel> <mode>");
	chan.setInviteMode(args[1][0] == '+');
	if (DEBUG)
	{
		if (chan.getInviteMode())
			printMode(chan, "+i", "Set invite-only");
		else
			printMode(chan, "-i", "Removed invite-only");
	}
	
}

void
ModeCommand::modeTopic(Client& client, Channel& chan,
						 const std::vector<std::string>& args)
{
	(void)client;
	requireArgsNum(args, 2, "MODE #<channel> <mode>");
	chan.setTopicMode(args[1][0] == '+');
	if (DEBUG)
	{
		if (chan.getTopicMode())
			printMode(chan, "+t", "Restricted TOPIC to operators");
		else
			printMode(chan, "-t", "Removed TOPIC restriction");
	}
}

void
ModeCommand::modeKeyOn(Client& client, Channel& chan,
					   const std::vector<std::string>& args)
{
	(void)client;
	requireArgsNum(args, 3, "MODE #<channel> <mode> <password>");
	requireWord(args, 2, "new password");
	chan.setKey(args[2]);
	if (DEBUG)
		printMode(chan, "+k", "Set password", args[2]);
}

void
ModeCommand::modeKeyOff(Client& client, Channel& chan,
						const std::vector<std::string>& args)
{
	(void)client;
	requireArgsNum(args, 2, "MODE #<channel> <mode>");
	chan.setKey("");
	if (DEBUG)
		printMode(chan, "-k", "Removed password");
}

void
ModeCommand::modeOpOn(Client& client, Channel& chan,
					  const std::vector<std::string>& args)
{
	requireArgsNum(args, 3, "MODE #<channel> <othernick>");
	requireWord(args, 2, "target nickname");
	if (client.getNickname() != args[2])
	{
		Client* target = Server::getInstance()->getClientByNick(args[2]);
		if (target)
		{
			if (!chan.isOperator(*target))
			{
				chan.addOperator(*target);
				if (DEBUG)
					printMode(chan, "+o", "Gave operator to", args[2]);
			}
		}
		else
			ServerReply::reply(client, ServerReply::ERR_NOSUCHNICK, args[2]);
	}
}

void
ModeCommand::modeOpOff(Client& client, Channel& chan,
					   const std::vector<std::string>& args)
{
	requireArgsNum(args, 3, "MODE #<channel> <othernick>");
	requireWord(args, 2, "target nickname");
	if (client.getNickname() != args[2])
	{
		Client* target = Server::getInstance()->getClientByNick(args[2]);
		if (target)
		{
			if (chan.isOperator(*target))
			{
				chan.removeOperator(*target);
				if (DEBUG)
					printMode(chan, "-o", "Removed operator from", args[2]);
			}
		}
		else
			ServerReply::reply(client, ServerReply::ERR_NOSUCHNICK, args[2]);
	}
}

void
ModeCommand::modeLimitOn(Client& client, Channel& chan,
						 const std::vector<std::string>& args)
{
	(void)client;
	requireArgsNum(args, 3, "MODE #<channel> <positive_int>");
	requirePosInt(args, 2, "channel limit");

	int limit;
	utils::ft_atou(args[2], limit);
	chan.setUserLimit(limit);
	if (DEBUG)
		printMode(chan, "+l", "Set user limit", args[2]);
}

void
ModeCommand::modeLimitOff(Client& client, Channel& chan,
						  const std::vector<std::string>& args)
{
	(void)client;
	requireArgsNum(args, 2, "MODE #<channel>");
	if (DEBUG)
		printMode(chan, "-l", "Removed user limit");
	chan.setUserLimit(0);
}
