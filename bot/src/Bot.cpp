/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:36:01 by ekeisler          #+#    #+#             */
/*   Updated: 2026/05/07 15:56:41 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "BotCommand.hpp"
#include "BotReply.hpp"
#include "BotSignals.hpp"

#include <iostream>
#include <stdexcept>

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

Bot::Bot(const std::string& host,
		 int				port,
		 const std::string& pass,
		 const std::string& nick) :
	_sockfd(-1),
	_nick(nick),
	_channel("#botchannel")
{
	this->_connect(host, port);
	this->_authenticate(pass);
}

Bot::~Bot(void)
{
	if (this->_sockfd != -1)
		close(this->_sockfd);
}

void
Bot::_send(const std::string& msg) const
{
	if (send(this->_sockfd, msg.c_str(), msg.size(), 0) == -1)
		throw std::runtime_error("Error: send() failed");
}

void
Bot::_connect(const std::string& host, int port)
{
	struct sockaddr_in addr;
	struct hostent*	   he;
	int				   opt = 1;

	memset(&addr, 0, sizeof(addr));
	he = gethostbyname(host.c_str());
	if (he == NULL)
		throw std::runtime_error("Error: gethostbyname() failed for: " + host);

	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd < 0)
		throw std::runtime_error("Error: socket() failed");

	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))
		< 0)
	{
		close(this->_sockfd);
		this->_sockfd = -1;
		throw std::runtime_error("Error: setsockopt(SO_REUSADDR) failed");
	}

	addr.sin_family = AF_INET;
	addr.sin_port	= htons(port);
	addr.sin_addr	= *reinterpret_cast<struct in_addr*>(he->h_addr_list[0]);

	if (connect(this->_sockfd,
				reinterpret_cast<struct sockaddr*>(&addr),
				sizeof(addr))
		< 0)
	{
		close(this->_sockfd);
		this->_sockfd = -1;
		throw std::runtime_error("Error: connect() failed");
	}

	std::cout << "[Bot] Connected to " << host << ":" << port << std::endl;
}

void
Bot::_authenticate(const std::string& pass)
{
	this->_send(BotReply::pass(pass));
	this->_send(BotReply::nick(this->_nick));
	this->_send(BotReply::user(this->_nick, "ft_irc bot"));

	std::cout << "[Bot] Authentification sent as" << this->_nick << std::endl;
}

void
Bot::_handlePrivmsg(const std::string& raw)
{
	std::string sender = BotCommand::getSender(raw);
	std::string target = BotCommand::getTarget(raw);
	std::string body   = BotCommand::getBody(raw);

	std::cout << "[Bot] PRIVMSG from " << sender << " to " << target << ": "
			  << body << std::endl;

	std::string replyTarget = (target == this->_nick) ? sender : target;

	if (body == "!ping")
		this->_send(BotReply::privmsg(replyTarget, "pong!"));
	else if (body == "!hello")
		this->_send(BotReply::privmsg(replyTarget, "Hello, " + sender + "!"));
	else if (body == "!help")
		this->_send(BotReply::privmsg(
			replyTarget, "Commands: !help | !ping | !hello | !feur"));
	else if (body == "!feur")
		this->_send(BotReply::privmsg(replyTarget, "Quoi ?"));
}

void
Bot::_loop(void)
{
	char		buf[512];
	std::string accumulator;

	while (g_bot_running)
	{
		std::memset(buf, 0, sizeof(buf));
		ssize_t bytes = recv(_sockfd, buf, sizeof(buf) - 1, 0);

		if (bytes < 0)
		{
			if (errno == EINTR)
				continue;
			std::cerr << "[Bot] recv() error." << std::endl;
			break;
		}
		if (bytes == 0)
		{
			std::cout << "[Bot] Server closed connection." << std::endl;
			break;
		}

		accumulator += std::string(buf, bytes);

		if (accumulator.size() > 4096)
		{
			std::cerr << "[Bot] Buffer overflow, clearing." << std::endl;
			accumulator.clear();
			continue;
		}

		std::size_t pos;
		while ((pos = accumulator.find("\r\n")) != std::string::npos)
		{
			std::string line = accumulator.substr(0, pos);
			accumulator.erase(0, pos + 2);

			std::cout << "[IRC] " << line << std::endl;

			std::string cmd = BotCommand::getCommand(line);

			try
			{
				if (cmd == "PRIVMSG")
					this->_handlePrivmsg(line);
				else if (cmd == "001")
					this->_send(BotReply::joinChannel(this->_channel));
			}
			catch (const std::exception& e)
			{
				std::cerr << "[Bot] send error: " << e.what() << '\n';
				return;
			}
		}
	}
	std::cout << "[Bot] Shutting down." << std::endl;
}

void
Bot::run(void)
{
	this->_loop();
}
