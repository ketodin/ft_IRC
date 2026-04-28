/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 17:36:26 by ekeisler          #+#    #+#             */
/*   Updated: 2026/04/28 18:23:26 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CapCommand.hpp"

const std::string CapCommand::NAME = "CAP";

void
CapCommand::execute(Client& client, // cppcheck-suppress constParameterReference
					const std::vector<std::string>& args)
{
	// Ignoring "CAP END" from irssi client sending his own answer
	if (args[0] == "END")
		return;

	requireArgsNum(args, 2, "CAP LS 302");
	requireWord(args, 0, "CAP");

	std::cout << "Client fd: " << client.getFd();
	if (!client.isRegistered())
	{
		std::string r = ":ircserv CAP * LS :\r\n";
		send(client.getFd(), r.c_str(), r.size(), 0);
	}
	std::cout << "\n";
}
