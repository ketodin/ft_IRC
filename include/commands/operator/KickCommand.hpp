/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:55 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/23 22:26:19 by jaubry--         ###   ########.fr       */
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

	public:
		static const std::string NAME; // = "KICK"

		// KICK <username>
		static void execute(const std::vector<std::string>& args);
};

#endif // KICK_COMMAND_HPP
