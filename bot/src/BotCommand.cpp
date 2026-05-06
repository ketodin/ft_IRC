/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:50:05 by ekeisler          #+#    #+#             */
/*   Updated: 2026/05/06 16:44:46 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BotCommand.hpp"

std::string
BotCommand::getCommand(const std::string& raw)
{
	std::string line = raw;

	std::size_t pos = 0;
	if (!line.empty() && (line[0] == ':'))
	{
		pos = line.find(' ');
		if (pos == std::string::npos)
			return ("");
		pos++;
	}

	std::size_t end = line.find(' ', pos);
	return (line.substr(pos, end - pos));
}

std::string
BotCommand::getSender(const std::string& raw)
{
	if (!raw.empty() && (raw[0] != ':'))
		return ("");

	std::size_t bang  = raw.find('!');
	std::size_t space = raw.find(' ');

	if (bang != std::string::npos && bang < space)
		return (raw.substr(1, bang - 1));

	if (space != std::string::npos)
		return (raw.substr(1, space - 1));

	return ("");
}

std::string
BotCommand::getBody(const std::string& raw)
{
	std::size_t colon = raw.find(" :");
	if (colon == std::string::npos)
		return ("");
	return (raw.substr(colon + 2));
}

// Returns the first parameter after COMMAND (usually target nick or channel)
std::string
BotCommand::getTarget(const std::string& raw)
{
	std::string cmd = getCommand(raw);
	if (cmd.empty())
		return ("");

	// find where the command token ends
	std::size_t pos = raw.find(cmd);
	if (pos == std::string::npos)
		return ("");
	pos += cmd.size();

	// skip the space after the command
	if (pos >= raw.size() || raw[pos] != ' ')
		return ("");
	pos++;

	// trailing starts with ':'  → no separate target
	if (pos >= raw.size() || raw[pos] == ':')
		return ("");

	std::size_t end = raw.find(' ', pos);
	return (raw.substr(pos, end - pos));
}
