/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:50:10 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/28 17:55:45 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"

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
	client.setPassAccepted(Server::getInstance()->isPasswordValid(password));

	if (client.getPassAccepted() && client.getNickSet() && client.getUserSet()
		&& !client.getRegistered())
	{
		const Server* instance = Server::getInstance();
		client.setRegistered(true);
		instance->sendWelcomeBurst(client);
	}
}
