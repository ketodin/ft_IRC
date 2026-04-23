/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:44:20 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/23 15:41:35 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <sstream>

int
main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "Error: invalid number of arguments\n"
				  << "Usage: " << argv[0] << " <port> <password>\n";
		return (1);
	}

	// TEST PARSING
	std::stringstream ss;
	int				  n;

	ss << argv[1];
	if (ss.fail())
	{
		std::cerr << "Parsing argv[1] failed" << std::endl;
	}
	ss >> n;

	// TESTING SERV INIT + CLIENT CONNEXION (nc -C 127.0.0.1 6667)
	try
	{
		Server serv(n, argv[2]);
		while (true)
		{
			int fd = serv.listenSockets();
			if (fd > 0)
				std::cout << "client accepted" << std::endl;
			else
				std::cout << "client not accepted" << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
