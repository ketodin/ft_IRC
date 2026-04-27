/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:50:10 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/28 01:33:54 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"
#include "utils.hpp"
#include <algorithm>

const std::string NickCommand::NAME = "NICK";

void
NickCommand::execute(Client& client, const std::vector<std::string>& args)
{
	requireArgsNum(args, 1, "NICK <nickname>");
	requireWord(args, 0, "nickname");

	if (checkOnUseNickname(args[0], Server::getInstance()->getClients()))
	{
		std::cout << "Nickname already taken" << std::endl;
		return;
	}

	client.setNickname(args[0]);

	std::cout << "Nickname of client " << client.getFd() << " is now "
			  << client.getNickname() << std::endl;
}

bool
NickCommand::checkOnUseNickname(const std::string&	 nickname,
								std::vector<Client*> registredClients)
{
	return (std::find_if(registredClients.begin(),
						 registredClients.end(),
						 utils::HasMemberValue<Client, std::string>(
							 &Client::getNickname, nickname))
			!= registredClients.end());
}
