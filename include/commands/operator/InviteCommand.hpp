/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:53 by jaubry--          #+#    #+#             */
/*   Updated: 2026/05/01 02:17:30 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITE_COMMAND_HPP
#define INVITE_COMMAND_HPP

#include "ACommand.hpp"
#include <string>
#include <vector>

class InviteCommand : public ACommand
{
	private:
		InviteCommand(void);
		InviteCommand(const InviteCommand& other);
		InviteCommand& operator=(const InviteCommand& other);

	public:
		static const std::string NAME; // = "INVITE"

		// INVITE <username>
		static void execute(Client&							client,
							const std::vector<std::string>& args);
		static bool checkInvitePossible(const Client&					client,
										const Client&					target,
										const Channel*					chan,
										const std::vector<std::string>& args);
};

#endif // INVITE_COMMAND_HPP
