/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 17:35:09 by ekeisler          #+#    #+#             */
/*   Updated: 2026/04/27 19:23:05 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAP_COMMAND_HPP
#define CAP_COMMAND_HPP

#include "ACommand.hpp"
#include <string>
#include <vector>

class CapCommand : public ACommand
{
	private:
		CapCommand(void);
		CapCommand(const CapCommand& other);
		CapCommand& operator=(const CapCommand& other);

	public:
		static const std::string NAME; // = "NICK"

		// Reply to "CAP <LS> <302>" (Tell to the client our features
		// implemented in the server)
		static void execute(Client&							client,
							const std::vector<std::string>& args);
};

#endif // NICK_COMMAND_HPP
