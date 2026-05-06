/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotReply.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:47:23 by ekeisler          #+#    #+#             */
/*   Updated: 2026/05/06 00:41:05 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTREPLY_HPP
#define BOTREPLY_HPP

#include <string>

class BotReply
{
	public:
		static std::string privmsg(const std::string& target,
								   const std::string& msg);
		static std::string joinChannel(const std::string& channel);
		static std::string pong(const std::string& token);
		static std::string nick(const std::string& nickname);
		static std::string user(const std::string& username,
								const std::string& realname);
		static std::string pass(const std::string& password);
};

#endif
