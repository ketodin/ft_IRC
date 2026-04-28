/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PongCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 00:28:32 by ekeisler          #+#    #+#             */
/*   Updated: 2026/04/29 00:41:31 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PongCommand.hpp"

const std::string PongCommand::NAME = "PING";

void
PongCommand::execute(Client& client, const std::vector<std::string>& args)
{
	// ERR_NOORIGIN (409) — no server token provided
	if (args.empty())
        return;
	client.reply("PONG " + args[0] + "\r\n");
}
