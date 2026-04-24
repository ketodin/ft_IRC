/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:57 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/23 22:51:20 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_COMMAND_HPP
#define MODE_COMMAND_HPP

#include "ACommand.hpp"
#include <map>
#include <string>
#include <vector>

typedef void (*ModeHandler)(const std::string&				channel,
							const std::vector<std::string>& args);

class ModeCommand : public ACommand
{
	private:
		ModeCommand(void);
		ModeCommand(const ModeCommand& other);
		ModeCommand& operator=(const ModeCommand& other);
		static std::map<std::string, ModeHandler>& getHandlers(void);

		static void modeInviteOn(const std::string&				 channel,
								 const std::vector<std::string>& args);
		static void modeInviteOff(const std::string&			  channel,
								  const std::vector<std::string>& args);
		static void modeTopicOn(const std::string&				channel,
								const std::vector<std::string>& args);
		static void modeTopicOff(const std::string&				 channel,
								 const std::vector<std::string>& args);
		static void modeKeyOn(const std::string&			  channel,
							  const std::vector<std::string>& args);
		static void modeKeyOff(const std::string&			   channel,
							   const std::vector<std::string>& args);
		static void modeOpOn(const std::string&				 channel,
							 const std::vector<std::string>& args);
		static void modeOpOff(const std::string&			  channel,
							  const std::vector<std::string>& args);
		static void modeLimitOn(const std::string&				channel,
								const std::vector<std::string>& args);
		static void modeLimitOff(const std::string&				 channel,
								 const std::vector<std::string>& args);

	public:
		static const std::string NAME; // = "MODE"

		// MODE <username>
		static void execute(const std::vector<std::string>& args);
};

#endif // MODE_COMMAND_HPP
