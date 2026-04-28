/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:44 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/28 15:39:56 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_COMMAND_HPP
#define USER_COMMAND_HPP

#include "ACommand.hpp"
#include <string>
#include <vector>

class UserCommand : public ACommand
{
	private:
		UserCommand(void);
		UserCommand(const UserCommand& other);
		UserCommand& operator=(const UserCommand& other);

	public:
		static const std::string NAME; // = "USER"

		// USER <username>
		static void execute(Client&							client,
							const std::vector<std::string>& args);
};

#endif // USER_COMMAND_HPP
