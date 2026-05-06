/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:46:45 by ekeisler          #+#    #+#             */
/*   Updated: 2026/05/06 00:49:28 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTCOMMAND_HPP
#define BOTCOMMAND_HPP

#include <string>

class BotCommand
{
	public:
		static std::string getCommand(const std::string& raw);
		static std::string getSender(const std::string& raw);
		static std::string getBody(const std::string& raw);
		static std::string getTarget(const std::string& raw);
};

#endif
