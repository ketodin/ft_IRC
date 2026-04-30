/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 21:00:50 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 06:53:14 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <Client.hpp>
#include <algorithm>
#include <sstream>

class Channel
{
	private:
		const std::string _name;
		std::string		  _topic;

		bool		 _inviteOnly;
		bool		 _topicRestricted;
		std::string	 _key;
		unsigned int _userLimit;

		std::vector<Client*> _members;
		std::vector<Client*> _operators;
		std::vector<Client*> _invites;

		static void sendMsg(const Client& client, const std::string& msg);

	public:
		explicit Channel(const std::string& name);
		~Channel(void);

		void addMember(const Client& client);
		void removeMember(const Client& client);
		bool isMember(const Client& client) const;

		void addOperator(const Client& client);
		void removeOperator(const Client& client);
		bool isOperator(const Client& client) const;

		void addInvite(const Client& client);
		bool isInvited(const Client& client) const;

		void broadcast(const std::string& msg, const Client* except = NULL);

		std::string getModeString(void) const;
		std::string buildNamesReply(void) const;

		std::string getName(void) const;

		std::string getTopic(void) const;
		void		setTopic(const std::string& topic);

		unsigned int getUserLimit(void) const;
		void		 setUserLimit(unsigned int);

		bool getInviteMode(void) const;
		void setInviteMode(bool);

		bool getTopicMode(void) const;
		void setTopicMode(bool);

		void setKey(const std::string& newKey);

		std::size_t getMemberSize(void) const;

		bool isValidKey(const std::string& key) const;
};

#endif // CHANNEL_HPP
