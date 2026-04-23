/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:44:20 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/23 20:06:24 by lcalero          ###   ########.fr       */
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

		serv.start();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
