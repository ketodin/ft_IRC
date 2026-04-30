/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 21:31:47 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 01:57:03 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCommand.hpp"

const std::string UserCommand::NAME = "USER";

void
UserCommand::execute(Client& client, const std::vector<std::string>& args)
{
	if (client.getRegistered())
	{
		ServerReply::reply(client, ServerReply::ERR_ALREADYREGISTERED);
		return ;
	}
	requireArgsNum(args, 4, "USER <username> <hostname> <servename> <realname>");
	requireWord(args, 0, "username");
	requireWord(args, 1, "hostname");
	requireWord(args, 2, "servename");
	requireStr(args, 3, "realname");

	client.setUsername(args[0]);
	client.setRealName(args[3]);
	client.setUserSet(true);

	if (client.firstRegistered())
	{
		client.setRegistered(true);
		Server::getInstance()->sendWelcomeBurst(client);
	}
}
