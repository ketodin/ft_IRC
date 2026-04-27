/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:08:01 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/28 00:04:07 by jaubry--         ###   ########.fr       */
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

		void printClientData() const;

	public:
		// explicit keyword to prevent from implicit conversions
		explicit Client(int fd, std::string hostname);
		~Client();

		std::vector<std::string> extractMessages();
		void					 appendToBuffer(const std::string& data);
		int						 getFd() const;
		bool					 getPassAccepted() const;
		void					 setPassAccepted(bool status);

		std::string getPrefix(void) const;
		bool		isRegistered(void) const;

		std::string getNickname(void) const;
};

#endif
