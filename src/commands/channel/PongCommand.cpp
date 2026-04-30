/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PongCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 00:28:32 by ekeisler          #+#    #+#             */
/*   Updated: 2026/04/30 01:46:25 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PongCommand.hpp"

const std::string PongCommand::NAME = "PING";

void
PongCommand::execute(
	Client& client, // cppcheck-suppress constParameterReference
	const std::vector<std::string>& args)
{
	if (args.empty())
	{
		ServerReply::reply(client, ServerReply::ERR_NOORIGIN);
		return;
	}
	client.reply("PONG " + args[0] + "\r\n");
}
