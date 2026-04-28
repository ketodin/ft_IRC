/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:50:10 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/28 04:28:01 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"
#include "utils.hpp"
#include <algorithm>
#include <sys/socket.h>

const std::string NickCommand::NAME = "NICK";

void
NickCommand::execute(Client& client, const std::vector<std::string>& args)
{
	if (args.empty())
	{
		// send ERR_NONICKNAMEGIVEN 431
		std::cout << "NICK command needs at least one argument" << std::endl;
		return;
	}
	requireWord(args, 0, "nickname");

	const Server* instance = Server::getInstance();
	if (instance->getClientByNick(args[0]))
	{
		// send ERR_NICKNAMEINUSE 433
		std::cout << "Nickname already taken" << std::endl;
		return;
	}

	client.setNickname(args[0]);

	if (client.isRegistered())
	{
		std::string message = ":" + client.getNickname() + "!"
							  + client.getUsername() + "@"
							  + client.getHostname() + " NICK " + args[0];
		instance->broadcast(message);
	}
}
