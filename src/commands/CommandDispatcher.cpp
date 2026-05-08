/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:11:23 by jaubry--          #+#    #+#             */
/*   Updated: 2026/05/08 18:41:37 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandDispatcher.hpp"
#include "Debug.hpp"
#include "utils.hpp"
#include <iostream>
#include <stdexcept>

CommandDispatcher::CommandDispatcher(void)
{
	registerCommand(PassCommand::NAME, &PassCommand::execute);
	registerCommand(NickCommand::NAME, &NickCommand::execute);
	registerCommand(CapCommand::NAME, &CapCommand::execute);
	registerCommand(UserCommand::NAME, &UserCommand::execute);
	registerCommand(PongCommand::NAME, &PongCommand::execute);
	registerCommand(JoinCommand::NAME, &JoinCommand::execute);
	registerCommand(TopicCommand::NAME, &TopicCommand::execute);
	registerCommand(PrivmsgCommand::NAME, &PrivmsgCommand::execute);
	registerCommand(ModeCommand::NAME, &ModeCommand::execute);
	registerCommand(InviteCommand::NAME, &InviteCommand::execute);
	registerCommand(KickCommand::NAME, &KickCommand::execute);
}

CommandDispatcher::~CommandDispatcher(void)
{
}

void
CommandDispatcher::registerCommand(const std::string& name,
								   CommandHandler	  handler)
{
	this->_handlers[name] = handler;
}

void
CommandDispatcher::displayCommand(const std::string&			  name,
								  const Client&					  client,
								  const std::vector<std::string>& args)
{
	if (DEBUG)
	{
		DBG_SEPARATOR;
		DBG_SECTION("CLIENT");
		client.printClientData();
		DBG_SECTION("COMMAND");
	}
	DBG_CMD(name);
	if (DEBUG)
	{
		for (std::size_t i = 0; i < args.size(); ++i)
			DBG_ARG(i, utils::escape_str(args[i]));
		DBG_SEPARATOR;
	}
}

void
CommandDispatcher::dispatch(Client&							client,
							const std::string&				name,
							const std::vector<std::string>& args) const
{
	HandlerMap::const_iterator it = this->_handlers.find(name);

	this->displayCommand(name, client, args);
	if (it == this->_handlers.end())
	{
		ServerReply::reply(client, ServerReply::ERR_UNKNOWNCOMMAND, name);
		return;
	}

	try
	{
		it->second(client, args);
	}
	catch (const ACommand::NeedMoreParamsException& e)
	{
		ServerReply::reply(client, ServerReply::ERR_NEEDMOREPARAMS, name);
	}
	catch (const ACommand::TooManyParamsException& e)
	{
		std::cout << "client sent a command with too many params\n";
	}
}
