/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PongCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 00:26:11 by ekeisler          #+#    #+#             */
/*   Updated: 2026/04/29 00:37:57 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PONGCOMMAND_HPP
#define PONGCOMMAND_HPP

#include "ACommand.hpp"

class PongCommand : public ACommand
{
	private:
		PongCommand(void);
		PongCommand(const PongCommand& other);
		PongCommand& operator=(const PongCommand& other);

	public:
		static const std::string NAME; // = "PASS"

		// PASS <password>
		static void execute(Client&							client,
							const std::vector<std::string>& args);
};

#endif
