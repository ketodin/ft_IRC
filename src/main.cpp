/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:44:20 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/23 19:40:21 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include <iostream>

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
