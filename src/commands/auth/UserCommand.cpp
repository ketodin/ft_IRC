/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 21:31:47 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/28 21:40:35 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCommand.hpp"

const std::string UserCommand::NAME = "USER";

void
UserCommand::execute(Client& client, const std::vector<std::string>& args)
{
	if (client.getRegistered())
	{
		// send ERR_ALREADYREGISTRED 462
		std::cout << "Client already registered" << std::endl;
		return;
	}

	try
	{
		requireArgsNum(
			args, 4, "USER <username> <hostname> <servename> <realname>");
	}
	catch (const std::exception& e)
	{
		// send ERR_NEEDMOREPARAMS 461
		std::cerr << e.what() << '\n';
		return;
	}

	client.setUsername(args[0]);
	client.setHostName(args[1]);
	client.setRealName(args[3]);
	client.setUserSet(true);

	if (client.getPassAccepted() && client.getNickSet() && client.getUserSet()
		&& !client.getRegistered())
	{
		const Server* instance = Server::getInstance();
		client.setRegistered(true);
		instance->sendWelcomeBurst(client);
	}
}
