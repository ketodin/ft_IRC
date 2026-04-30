/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:50:10 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 00:14:33 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"

const std::string PassCommand::NAME = "PASS";

void
PassCommand::execute(Client& client, const std::vector<std::string>& args)
{
	requireArgsNum(args, 1, "PASS <password>");
	requireWord(args, 0, "password");

	if (client.getRegistered())
	{
		ServerReply::reply(client, ServerReply::ERR_ALREADYREGISTERED);
		return ;
	}
	const std::string& password = args[0];

	const bool valid = Server::getInstance()->isPasswordValid(password);
	client.setPassAccepted(valid);
	if (!valid)
	{
		ServerReply::reply(client, ServerReply::ERR_PASSWDMISMATCH);
		return ;
	}

	if (client.firstRegistered())
	{
		client.setRegistered(true);
		Server::getInstance()->sendWelcomeBurst(client);
	}
}
