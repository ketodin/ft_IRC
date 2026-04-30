/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerReply.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 23:41:41 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 01:45:54 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_REPLY_HPP
#define SERVER_REPLY_HPP

#include "Server.hpp"

namespace ServerReply
{
enum Code
{
	// Registration
	RPL_WELCOME	 = 1,
	RPL_YOURHOST = 2,
	RPL_CREATED	 = 3,
	RPL_MYINFO	 = 4,

	// Channel info
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC		  = 331,
	RPL_TOPIC		  = 332,
	RPL_INVITING	  = 341,
	RPL_NAMREPLY	  = 353,
	RPL_ENDOFNAMES	  = 366,

	// Errors
	ERR_NOSUCHNICK		  = 401,
	ERR_NOORIGIN		  = 409,
	ERR_NORECIPIENT		  = 411,
	ERR_NOTEXTTOSEND	  = 412,
	ERR_UNKNOWNCOMMAND	  = 421,
	ERR_NONICKNAMEGIVEN	  = 431,
	ERR_ERRONEUSNICKNAME  = 432,
	ERR_NICKNAMEINUSE	  = 433,
	ERR_USERNOTINCHANNEL  = 441,
	ERR_NOTONCHANNEL	  = 442,
	ERR_USERONCHANNEL	  = 443,
	ERR_NOTREGISTERED	  = 451,
	ERR_NEEDMOREPARAMS	  = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH	  = 464,
	ERR_CHANNELISFULL	  = 471,
	ERR_UNKNOWNMODE		  = 472,
	ERR_INVITEONLYCHAN	  = 473,
	ERR_BADCHANNELKEY	  = 475,
	ERR_CHANOPRIVSNEEDED  = 482
};

// std::string makeReply(const Code& code, const Client& client, const Channel&
// channel, const std::string& extra);

void reply(const Client& client, const Code& code);
void reply(const Client& client, const Code& code, const std::string& extra);
void reply(const Client& client, const Channel& channel, const Code& code);
void reply(const Client&	  client,
		   const Channel&	  channel,
		   const Code&		  code,
		   const std::string& extra);
}

#endif // SERVER_REPLY_HPP
