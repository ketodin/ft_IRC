/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:08:01 by lcalero           #+#    #+#             */
/*   Updated: 2026/05/08 18:32:07 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>

class Client
{
	private:
		// attributed fd
		int _fd;

		// all client names that will later be used in the commands
		std::string _hostname;
		std::string _nickname;
		std::string _username;
		std::string _realname;

		// raw buffer to receive data
		std::string _inputBuffer;

		// has entered correct password
		bool _passAccepted;
		bool _nickSet;
		bool _userSet;
		bool _registered;

	public:
		// explicit keyword to prevent from implicit conversions
		explicit Client(int fd, std::string hostname);
		~Client();

		std::vector<std::string> extractMessages();
		void					 appendToBuffer(const std::string& data);
		void					 setPassAccepted(bool status);
		void					 setNickname(const std::string& nickname);
		void					 setNickSet(bool nickset);
		void					 setUsername(const std::string& username);
		void					 setUserSet(bool userset);
		void					 setRealName(const std::string& realname);
		void					 setRegistered(bool registered);
		void					 reply(const std::string& msg) const;

		std::string getNickname(void) const;
		std::string getPrefix(void) const;
		bool		getRegistered(void) const;
		int			getFd(void) const;
		bool		firstRegistered(void) const;

		void printClientData() const;
};

#endif
