/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:11:23 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 01:21:16 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandDispatcher.hpp"
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

	/*
	registerCommand(PrivmsgCommand::NAME, &PrivmsgCommand::execute);

	registerCommand(InviteCommand::NAME, &InviteCommand::execute);
	registerCommand(KickCommand::NAME, &KickCommand::execute);
	registerCommand(ModeCommand::NAME, &ModeCommand::execute);
	registerCommand(TopicCommand::NAME, &TopicCommand::execute);
	*/
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
								  const std::vector<std::string>& args)
{
	std::cout << "cmd: [" << name << "]\n";
	for (std::size_t i = 0; i < args.size(); ++i)
		std::cout << "arg" << i << ": `" << utils::escape_str(args[i]) << "`\n";
}

void
CommandDispatcher::dispatch(Client&							client,
							const std::string&				name,
							const std::vector<std::string>& args) const
{
	HandlerMap::const_iterator it = this->_handlers.find(name);

	this->displayCommand(name, args);
	if (it == this->_handlers.end())
	{
		ServerReply::reply(client, ServerReply::ERR_UNKNOWNCOMMAND, name);
		return ;
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
