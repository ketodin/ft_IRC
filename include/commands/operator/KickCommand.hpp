/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:55 by jaubry--          #+#    #+#             */
/*   Updated: 2026/05/05 11:13:29 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_COMMAND_HPP
#define KICK_COMMAND_HPP

#include "ACommand.hpp"
#include <string>
#include <vector>

class KickCommand : public ACommand
{
	private:
		KickCommand(void);
		KickCommand(const KickCommand& other);
		KickCommand& operator=(const KickCommand& other);

		static bool isValidKick(const Client&  client,
								const Client&  target,
								const Channel& chan);

	public:
		static const std::string NAME; // = "KICK"

		// KICK <username>
		static void execute(Client&							client,
							const std::vector<std::string>& args);
};

#endif // KICK_COMMAND_HPP
