/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:36:04 by ekeisler          #+#    #+#             */
/*   Updated: 2026/05/06 01:37:32 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>

bool
validNickname(const std::string& str)
{
	return (str.find_first_of(std::string("\x00\r\n :#", 6))
			== std::string::npos);
}

static int
parsePort(const std::string& port_str)
{
	char* end;
	errno	  = 0;
	long port = std::strtol(port_str.c_str(), &end, 10);

	if (errno != 0)
		throw std::runtime_error("Error: port overflow: " + port_str);
	if (*end != '\0')
		throw std::runtime_error("Error: port contains non-numeric characters: "
								 + port_str);
	if (end == port_str.c_str())
		throw std::runtime_error("Error: port is empty");
	if (port < 1 || port > 65535)
		throw std::runtime_error("Error: port out of range (1-65535): "
								 + port_str);

	return (static_cast<int>(port));
}

void
checkArgs(const std::string& host,
		  const std::string& pass,
		  const std::string& nick)
{
	if (host.empty())
	{
		throw std::runtime_error("Error: host is empty");
	}

	if (pass.empty())
	{
		throw std::runtime_error("Error: pass is empty");
	}

	if (nick.empty())
	{
		throw std::runtime_error("Error: nick is empty");
	}

	if (!validNickname(nick))
	{
		throw std::runtime_error(
			"Error: invalid characters in the nickname entered");
	}
}

int
main(int argc, const char* argv[])
{
	if (argc != 5)
	{
		std::cerr << "Error: invalid number of arguments\n"
				  << "Usage: <host> <port> <password> <nickname>\n";
		return (1);
	}

	std::string host = argv[1];
	std::string pass = argv[3];
	std::string nick = argv[4];

	try
	{
		checkArgs(host, pass, nick);
		int port = parsePort(argv[2]);
		Bot newBot(host, port, pass, nick);
		newBot.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
