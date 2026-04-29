/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:40 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/29 01:49:37 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_COMMAND_HPP
#define JOIN_COMMAND_HPP

#include "ACommand.hpp"
#include <string>
#include <vector>

class JoinCommand : public ACommand
{
	private:
		JoinCommand(void);
		JoinCommand(const JoinCommand& other);
		JoinCommand& operator=(const JoinCommand& other);

		static bool checkAccess(const Client&					client,
								const Channel&					chan,
								const std::vector<std::string>& args);

	public:
		static const std::string NAME; // = "JOIN"

		// JOIN <username>
		static void execute(Client&							client,
							const std::vector<std::string>& args);
};

#endif // JOIN_COMMAND_HPP
