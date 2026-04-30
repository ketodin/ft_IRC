/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:53 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 21:15:18 by lcalero          ###   ########.fr       */
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
};

#endif // INVITE_COMMAND_HPP
