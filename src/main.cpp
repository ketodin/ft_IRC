/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:44:20 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/24 14:30:25 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <vector>
#include <string>

int
main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "Error: invalid number of arguments\n"
				  << "Usage: " << argv[0] << " <port> <password>\n";
		return (1);
	}

	// TESTING SERV INIT + CLIENT CONNEXION (nc -C 127.0.0.1 6667)
	try
	{
		int port = Server::parsePort(argv[1]);

		std::string password(argv[2]);
		if (password.empty())
		{
			std::cerr << "Error: password cannot be empty\n";
			return (1);
		}

		Server serv(port, password);
		Client randomClient(41);

		// TESTING CONSTRUCTING CLIENT MESSAGE
		randomClient.appendToBuffer("PRIV");
		randomClient.appendToBuffer("MSG #chan :hello\r\n");
		randomClient.appendToBuffer("NICK terr");
		randomClient.appendToBuffer("ale\r\n");
		randomClient.appendToBuffer("PASS incomplete");

		std::vector<std::string> messages = randomClient.extractMessages();

		for (size_t i = 0; i < messages.size(); i++)
        	std::cout << "[msg " << i << "] " << messages[i] << std::endl;

		randomClient.appendToBuffer("\r\n");
    	messages = randomClient.extractMessages();

    	// Should print 1 message: "PASS incomplete"
    	for (size_t i = 0; i < messages.size(); i++)
		{
        	std::cout << "[msg " << i << "] " << messages[i] << std::endl;
		}

		while (true)
		{
			int fd = serv.listenSockets();
			if (fd > 0)
				std::cout << "client accepted" << std::endl;
			else
			{
				std::cerr << "client not accepted" << std::endl;
				return (1);
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
