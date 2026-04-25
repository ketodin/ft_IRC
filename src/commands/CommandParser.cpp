/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:15:26 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/25 21:16:42 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandParser.hpp"
#include <cctype>
#include <stdexcept>

CommandParser::CommandParser(CommandDispatcher& dispatcher) :
	_dispatcher(dispatcher)
{
}

CommandParser::~CommandParser(void)
{
}

void
CommandParser::parse(Client& client, const std::string& line) const
{
	const std::string trimmed = this->trim(line);

	if (trimmed.empty())
		return;

	const std::vector<std::string> tokens = this->tokenise(trimmed);

	if (tokens.empty())
		return;

	const std::vector<std::string> args(tokens.begin() + 1, tokens.end());

	this->_dispatcher.dispatch(client, tokens[0], args);
}

std::vector<std::string>
CommandParser::tokenise(const std::string& line)
{
	std::vector<std::string> tokens;

	std::size_t		  i	  = 0;
	const std::size_t len = line.size();

	while (i < len)
	{
		while ((i < len) && (line[i] == ' '))
			++i;

		if (i >= len)
			break;

		if (line[i] == ':')
		{
			tokens.push_back(line.substr(i + 1));
			break;
		}

		std::size_t start = i;
		while ((i < len) && (line[i] != ' '))
			++i;

		tokens.push_back(line.substr(start, i - start));
	}
	return (tokens);
}

std::string
CommandParser::trim(const std::string& s)
{
	if (s.empty())
		return (s);

	std::size_t start = 0;
	while ((start < s.size())
		   && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r'
			   || s[start] == '\n'))
		++start;
	if (start == s.size())
		return ("");

	std::size_t end = s.size() - 1;
	while ((end > start)
		   && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r'
			   || s[end] == '\n'))
		--end;
	return (s.substr(start, end - start + 1));
}
