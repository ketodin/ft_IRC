/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:59 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 01:58:51 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPIC_COMMAND_HPP
#define TOPIC_COMMAND_HPP

#include "ACommand.hpp"
#include <string>
#include <vector>

class TopicCommand : public ACommand
{
	private:
		TopicCommand(void);
		TopicCommand(const TopicCommand& other);
		TopicCommand& operator=(const TopicCommand& other);

		static bool checkChannelAccess(const Client&				   client,
									   const Channel*				   Channel,
									   const std::vector<std::string>& args);

	public:
		static const std::string NAME; // = "TOPIC"

		// TOPIC <username>
		static void execute(Client&							client,
							const std::vector<std::string>& args);
};

#endif // TOPIC_COMMAND_HPP
