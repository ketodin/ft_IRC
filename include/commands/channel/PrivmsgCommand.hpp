/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:21 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/23 22:25:32 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_COMMAND_HPP
#define PRIVMSG_COMMAND_HPP

#include "ACommand.hpp"
#include <string>
#include <vector>

class PrivmsgCommand : public ACommand
{
	private:
		PrivmsgCommand(void);
		PrivmsgCommand(const PrivmsgCommand& other);
		PrivmsgCommand& operator=(const PrivmsgCommand& other);

	public:
		static const std::string NAME; // = "PRIVMSG"

		// PRIVMSG <username>
		static void execute(const std::vector<std::string>& args);
};

#endif // PRIVMSG_COMMAND_HPP
