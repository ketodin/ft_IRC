/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:08:01 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/25 21:43:19 by jaubry--         ###   ########.fr       */
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
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _hostname;

		// raw buffer to receive data
		std::string _inputBuffer;

		// has entered correct password
		bool _isLogged;

		void printClientData();

	public:
		// explicit keyword to prevent from implicit conversions
		explicit Client(int fd);
		~Client();

		std::vector<std::string> extractMessages();
		void					 appendToBuffer(const std::string& data);
		int						 getFd() const;
		bool					 getIsLogged() const;
		void					 setIsLogged(bool status);
};

#endif
