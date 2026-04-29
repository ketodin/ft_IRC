/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:50:10 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/29 01:49:30 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"
#include "ServerReply.hpp"

const std::string PassCommand::NAME = "PASS";

void
PassCommand::execute(Client& client, const std::vector<std::string>& args)
{
	requireArgsNum(args, 1, "PASS <password>");
	requireWord(args, 0, "password");

	const std::string& password = args[0];

	std::cout << "[PASS] authenticating with password: " << password
			  << std::endl;

	std::cout << "Client fd: " << client.getFd() << "\n";
	const bool valid = Server::getInstance()->isPasswordValid(password);
	client.setPassAccepted(valid);
	if (!valid)
		ServerReply::reply(client, ServerReply::ERR_PASSWDMISMATCH);

	if (client.getPassAccepted() && client.getNickSet() && client.getUserSet()
		&& !client.getRegistered())
	{
		const Server* instance = Server::getInstance();
		client.setRegistered(true);
		instance->sendWelcomeBurst(client);
	}
}
