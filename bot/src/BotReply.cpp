/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotReply.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:50:34 by ekeisler          #+#    #+#             */
/*   Updated: 2026/05/07 16:27:25 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BotReply.hpp"

std::string
BotReply::privmsg(const std::string& target, const std::string& msg)
{
	return ("PRIVMSG " + target + " :" + msg + "\r\n");
}

std::string
BotReply::joinChannel(const std::string& channel)
{
	return ("JOIN " + channel + "\r\n");
}

std::string
BotReply::nick(const std::string& nickname)
{
	return ("NICK " + nickname + "\r\n");
}

std::string
BotReply::user(const std::string& username, const std::string& realname)
{
	return ("USER " + username + " 0 * :" + realname + "\r\n");
}

std::string
BotReply::pass(const std::string& password)
{
	return ("PASS " + password + "\r\n");
}
