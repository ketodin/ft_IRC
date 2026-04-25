/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:40 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/23 22:25:01 by jaubry--         ###   ########.fr       */
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

	public:
		static const std::string NAME; // = "JOIN"

		// JOIN <username>
		static void execute(const std::vector<std::string>& args);
};

#endif // JOIN_COMMAND_HPP
