/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommandException.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 00:43:17 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 00:55:31 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

ACommand::CommandException::CommandException(const std::string& msg) :
	std::runtime_error("[Command Error]: " + msg)
{
}

ACommand::CommandException::~CommandException(void) throw()
{
}

ACommand::CommandParamsException::CommandParamsException(
	const std::string& cause, const std::string& usage) :
	CommandException("Invalid params number, " + cause + ". Usage: " + usage)
{
}

ACommand::NeedMoreParamsException::NeedMoreParamsException(
	const std::string& usage) :
	CommandParamsException("not enough parameters", usage)
{
}

ACommand::TooManyParamsException::TooManyParamsException(
	const std::string& usage) :
	CommandParamsException("too many parameters", usage)
{
}
