/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:50:10 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/28 02:22:55 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"
#include "utils.hpp"
#include <algorithm>

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
	if (checkRegisteredNicknames(args[0], Server::getInstance()->getClients()))
	{
		// send ERR_NICKNAMEINUSE 433
		std::cout << "Nickname already taken" << std::endl;
		return;
	}

	client.setNickname(args[0]);

	std::cout << "Nickname of client " << client.getFd() << " is now "
			  << client.getNickname() << std::endl;
}

bool
NickCommand::checkRegisteredNicknames(const std::string&   nickname,
									  std::vector<Client*> registredClients)
{
	return (std::find_if(registredClients.begin(),
						 registredClients.end(),
						 utils::HasMemberValue<Client, std::string>(
							 &Client::getNickname, nickname))
			!= registredClients.end());
}
