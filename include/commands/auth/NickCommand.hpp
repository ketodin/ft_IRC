/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:49 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/27 22:42:28 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_COMMAND_HPP
#define NICK_COMMAND_HPP

#include "ACommand.hpp"
#include <string>
#include <vector>

class NickCommand : public ACommand
{
	private:
		NickCommand(void);
		NickCommand(const NickCommand& other);
		NickCommand& operator=(const NickCommand& other);

		static bool checkOnUseNickname(const std::string& nickname);

	public:
		static const std::string NAME; // = "NICK"

		// NICK <nickname>
		static void execute(Client&							client,
							const std::vector<std::string>& args);
};

#endif // NICK_COMMAND_HPP
