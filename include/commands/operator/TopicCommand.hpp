/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:59 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/23 22:26:39 by jaubry--         ###   ########.fr       */
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

	public:
		static const std::string NAME; // = "TOPIC"

		// TOPIC <username>
		static void execute(const std::vector<std::string>& args);
};

#endif // TOPIC_COMMAND_HPP
