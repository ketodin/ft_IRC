/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 21:49:24 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/29 00:09:28 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivmsgCommand.hpp"

const std::string PrivmsgCommand::NAME = "PRIVMSG";

void
PrivmsgCommand::execute(Client& client,
					const std::vector<std::string>& args)
{
	// ERR_NOTREGISTERED (451) — client has not completed registration
	if (!client.getRegistered())
		return;

	// ERR_NEEDMOREPARAMS (461) — missing target or message
	requireArgsNum(args, 2, "PRIVMSG <target> :<message>");

	// ERR_NORECIPIENT (411) — target is empty
	if (args[0].empty())
		return;
    // ERR_NOTEXTTOSEND (412) — message body is empty
	if (args[1].empty())
		return;

	const std::string&	target = args[0];
	const std::string&	msg = args[1];
	const std::string	prefix = ":" + client.getPrefix();

	const Server* instance = Server::getInstance();
	if (target[0] == '#')
	{
		Channel* chan = instance->getChannelByName(target);

		// ERR_NOSUCHCHANNEL (403) — channel does not exist
		if (!chan)
			return;

		// ERR_CANNOTSENDTOCHAN (404) — client is not a member of the channel
		if (!chan->isMember(client))
			return;
		chan->broadcast(prefix + " PRIVMSG " + target + " :" + msg + "\r\n", &client);
	}
	else
	{
		Client *dest = instance->getClientByNick(target);

		// ERR_NOSUCHNICK (401) — target nickname does not exist on the server
		if (!dest)
			return;

		dest->reply(prefix + " PRIVMSG " + target +" :" + msg + "\r\n");
	}
}
