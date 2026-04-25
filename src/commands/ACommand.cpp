/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:00:04 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/25 02:55:41 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"
#include "utils.hpp"
#include <sstream>
#include <stdexcept>

/*
	Require precise number of arguments
*/
void
ACommand::requireArgsNum(const std::vector<std::string>& args,
						 std::size_t					 num,
						 const std::string&				 usage)
{
	if (args.size() != num)
		throw std::invalid_argument("Wrong arguments number. Usage: " + usage);
}

/*
	Require at least min argument
	Require less than max argument
*/
void
ACommand::requireArgsNum(const std::vector<std::string>& args,
						 std::size_t					 min,
						 std::size_t					 max,
						 const std::string&				 usage)

{
	if (args.size() < min)
		throw std::invalid_argument("Not enough arguments. Usage: " + usage);
	else if (args.size() > max)
		throw std::invalid_argument("Too many arguments. Usage: " + usage);
}

/*
	Single word with no spaces newline or column, and must have a '#'
*/
bool
ACommand::validChannel(const std::string& str)
{
	return (
		(str.find_first_of(std::string("\x00\r\n :", 5)) == std::string::npos)
		&& (str[0] == '#'));
}

/*
	Single word with no spaces or newline that refuses : and # inside it.
*/
bool
ACommand::validWord(const std::string& str)
{
	return (str.find_first_of(std::string("\x00\r\n :#", 6))
			== std::string::npos);
}

/*
	String accepts spaces and backlines.
*/
bool
ACommand::validStr(const std::string& str)
{
	return (str.find_first_of(std::string("\x00\r", 2)) == std::string::npos);
}

/*
	Checks for `+` or `-` and specific character for mode selection
*/
bool
ACommand::validMode(const std::string& str)
{
	return ((str.size() == 2) && ((str[0] == '+') || (str[0] == '-'))
			&& (std::string("itkol").find(str[1]) != std::string::npos));
}

void
ACommand::requireMode(const std::vector<std::string>& args,
					  std::size_t					  index,
					  const std::string&			  argName)

{
	if (!validMode(args[index]))
		throw std::invalid_argument("Argument '" + argName
									+ "' has invalid characters.");
}

/*
	Should also check for invalid character for channel specific
*/
void
ACommand::requireChannel(const std::vector<std::string>& args,
						 std::size_t					 index,
						 const std::string&				 argName)

{
	if (!validChannel(args[index]))
		throw std::invalid_argument("Argument '" + argName
									+ "' has invalid characters.");
}

void
ACommand::requireWord(const std::vector<std::string>& args,
					  std::size_t					  index,
					  const std::string&			  argName)

{
	if (!validWord(args[index]))
		throw std::invalid_argument("Argument '" + argName
									+ "' has invalid characters.");
}

void
ACommand::requireStr(const std::vector<std::string>& args,
					 std::size_t					 index,
					 const std::string&				 argName)
{
	if (!validStr(args[index]))
		throw std::invalid_argument("Argument '" + argName
									+ "' has invalid characters.");
}

void
ACommand::optionalWord(const std::vector<std::string>& args,
					   std::size_t					   index,
					   const std::string&			   argName)
{
	if (args.size() > index)
		requireWord(args, index, argName);
}

void
ACommand::optionalParam(const std::vector<std::string>& args,
						std::size_t						index,
						const std::string&				argName)
{
	if (args.size() > index)
		requireStr(args, index, argName);
}

void
ACommand::requirePosInt(const std::vector<std::string>& args,
						std::size_t						index,
						const std::string&				argName)
{
	int num;

	utils::ft_atou(args[index], num);
	if (num < 1)
		throw std::invalid_argument("Argument '" + argName + "' is too low");
}
