/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:08:01 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/23 17:34:36 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

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

		// writing and reading buffer of the clients
		std::string _readBuffer;
		std::string _writeBuffer;

		void printClientData();

	public:
		// explicit keyword to prevent from implicit conversions
		explicit Client(int fd);
		~Client();
};

#endif
