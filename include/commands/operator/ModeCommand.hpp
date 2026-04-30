/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:57 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 05:11:14 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_COMMAND_HPP
#define MODE_COMMAND_HPP

#include "ACommand.hpp"
#include <map>
#include <string>
#include <vector>

typedef void (*ModeHandler)(Client&							client,
							Channel&						channel,
							const std::vector<std::string>& args);

class ModeCommand : public ACommand
{
	private:
		ModeCommand(void);
		ModeCommand(const ModeCommand& other);
		ModeCommand& operator=(const ModeCommand& other);
		static std::map<std::string, ModeHandler>& getHandlers(void);

		static void modeInvite(Client&						   client,
							   Channel&						   channel,
							   const std::vector<std::string>& args);
		static void modeTopic(Client&						  client,
							  Channel&						  channel,
							  const std::vector<std::string>& args);
		static void modeKeyOn(Client&						  client,
							  Channel&						  channel,
							  const std::vector<std::string>& args);
		static void modeKeyOff(Client&						   client,
							   Channel&						   channel,
							   const std::vector<std::string>& args);
		static void modeOpOn(Client&						 client,
							 Channel&						 channel,
							 const std::vector<std::string>& args);
		static void modeOpOff(Client&						  client,
							  Channel&						  channel,
							  const std::vector<std::string>& args);
		static void modeLimitOn(Client&							client,
								Channel&						channel,
								const std::vector<std::string>& args);
		static void modeLimitOff(Client&						 client,
								 Channel&						 channel,
								 const std::vector<std::string>& args);

	public:
		static const std::string NAME; // = "MODE"

		// MODE <username>
		static void execute(Client&							client,
							const std::vector<std::string>& args);
};

#endif // MODE_COMMAND_HPP
