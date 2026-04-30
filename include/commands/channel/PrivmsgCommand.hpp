/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:21 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 01:21:02 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_COMMAND_HPP
#define PRIVMSG_COMMAND_HPP

#include "ACommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <string>
#include <vector>

class PrivmsgCommand : public ACommand
{
	private:
		PrivmsgCommand(void);
		PrivmsgCommand(const PrivmsgCommand& other);
		PrivmsgCommand& operator=(const PrivmsgCommand& other);

		static bool validateArgs(const Client&					 client,
								 const std::vector<std::string>& args);

		static void sendToChannel(const Client&		 client,
								  const std::string& target,
								  const std::string& msg,
								  const std::string& prefix);

		static void sendToClient(const Client&		client,
								 const std::string& target,
								 const std::string& msg,
								 const std::string& prefix);

	public:
		static const std::string NAME; // = "PRIVMSG"

		// PRIVMSG <username>
		static void execute(Client&							client,
							const std::vector<std::string>& args);
};

#endif // PRIVMSG_COMMAND_HPP
