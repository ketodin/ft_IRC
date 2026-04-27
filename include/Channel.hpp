/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 21:00:50 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/27 22:33:42 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
#include "Server.hpp"
#include <algorithm>

class	Channel
{
	private:
		const std::string	_name;
		std::string	_topic;

		bool	_inviteOnly;
		bool	_topicRestricted;
		std::string	_key;
		unsigned int	_userLimit;

		std::vector<Client*>	_members;
		std::vector<Client*>	_operators;
		std::vector<Client*>	_invites;

	public:
		Channel(const std::string& name);
		~Channel(void);

		void	addMember(const Client& client);
		void	removeMember(const Client& client);
		bool	isMember(const Client& client) const;

		void	addOperator(const Client& client);
		void	removeOperator(const Client& client);
		bool	isOperator(const Client& client) const;

		void	addInvite(const Client& client);
		bool	isInvited(const Client& client) const;

		void	broadcast(const std::string& msg, const Client* except = NULL);
		std::string	getModeString(void) const;
		std::string	buildNamesReply(void) const;
};

#endif // CHANNEL_HPP
