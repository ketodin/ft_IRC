/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:46 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/23 17:36:45 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_COMMAND_HPP
#define PASS_COMMAND_HPP

#include "ACommand.hpp"
#include <string>
#include <vector>

class PassCommand : public ACommand
{
	private:
		PassCommand(void);
		PassCommand(const PassCommand& other);
		PassCommand& operator=(const PassCommand& other);

	public:
		static const std::string NAME; // = "PASS"

		// PASS <password>
		static void execute(const std::vector<std::string>& args);
};

#endif // PASS_COMMAND_HPP
