/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:52:35 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/30 03:37:39 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "CommandDispatcher.hpp"
#include "CommandParser.hpp"
#include "signals.hpp"
#include "utils.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

Server* Server::_instance = NULL; // NULL, not nullptr (C++98)

Server*
Server::getInstance(void)
{
	if (_instance == NULL)
		throw std::runtime_error(
			"Server not initialized. Call Server::init() first.");
	return _instance;
}

void
Server::init(int port, const std::string& password)
{
	if (_instance != NULL)
		throw std::runtime_error("Server already initialized.");
	_instance = new Server(port, password);
}

void
Server::destroy(void)
{
	delete _instance;
	_instance = NULL;
}

Server::Server(int port, const std::string& password) :
	_serverName("ft_irc.42lyon.fr"),
	_creationDate(utils::getCurrentTime()),
	_epoll_fd(-1),
	_listen_sock(-1),
	_port(port),
	_password(password),
	_clients(),
	_channels()
{
	// logging
	LOG_INFO(this->_port);
	LOG_INFO(this->_password);
	LOG_INFO(this->_epoll_fd);
	LOG_INFO(this->_listen_sock);

	// logic
	this->_clients.reserve(MAX_EVENTS);
	this->setupSocket();
}

Server::~Server(void)
{
	// closing opened fds
	if (this->_listen_sock != -1)
		close(this->_listen_sock);
	if (this->_epoll_fd != -1)
		close(this->_epoll_fd);

	// deleting client pointers
	for (std::vector<Client*>::const_iterator i = this->_clients.begin();
		 i != this->_clients.end();
		 ++i)
		delete (*i);
	// deleting channels pointers
	for (std::vector<Channel*>::const_iterator i = this->_channels.begin();
		 i != this->_channels.end();
		 ++i)
		delete (*i);
}

void
Server::broadcast(const std::string& msg, const Client* except)
{
	std::string finalMessage = msg + "\r\n";
	for (std::vector<Client*>::const_iterator it = this->_clients.begin();
		 it != this->_clients.end();
		 ++it)
	{
		const Client* currentClient = *it;

		if (!except)
			this->sendMsg(*currentClient, finalMessage);
		else if (currentClient->getFd() != except->getFd())
			this->sendMsg(*currentClient, finalMessage);
	}
}

void
Server::sendMsg(const Client& client, const std::string& msg)
{
	ssize_t sent = send(client.getFd(), msg.c_str(), msg.size(), 0);

	if (sent == -1)
		std::cout << "send(): failed" << std::endl;
	else if (sent < static_cast<ssize_t>(msg.size()))
		std::cout << "send(): message partially sent" << std::endl;
}

/* This function creates the listening socket of the server and binds
it to an adress */
void
Server::setupSocket(void)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	int opt = 1;

	this->_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_listen_sock < 0)
		throw CreateSocketException();

	if (setsockopt(
			this->_listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw SocketOptException("SO_REUSEADDR");

	addr.sin_family		 = AF_INET;
	addr.sin_port		 = htons(this->_port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_listen_sock,
			 reinterpret_cast<struct sockaddr*>(&addr),
			 sizeof(addr))
		< 0)
		throw BindException(this->_port);

	if (listen(this->_listen_sock, MAX_EVENTS) < 0)
		throw ListenException(this->_port);

	LOG_INFO("Server listening on port " << _port);
}

/* This function tries to accept a client and returns his associated fd */
void
Server::acceptClient(int& clientFd, std::string& clientHostname)
{
	struct sockaddr_in clientAddr;
	socklen_t		   clientLen = sizeof(clientAddr);

	clientFd = accept(this->_listen_sock,
					  reinterpret_cast<struct sockaddr*>(&clientAddr),
					  &clientLen);
	if (clientFd < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return; // no client actually ready, not a real error
		throw AcceptException();
	}
	char ipBuf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuf, sizeof(ipBuf));
	clientHostname = std::string(ipBuf);

	// clientAddr currently unused - will be needed for IP logging/banning

	LOG_INFO("New client connected, fd=" << clientFd);
}

/* This function parses the port and checks its validity
(first parameter of the binary) */
int
Server::parsePort(const char* str)
{
	char* end;
	long  n;

	for (int i = 0; str[i]; i++)
	{
		if (str[i] < '0' || str[i] > '9')
			throw PortNumberException(std::string(str));
	}

	errno = 0;
	n	  = std::strtol(str, &end, 10);

	if ((errno == ERANGE) || (n <= 0) || (n > MAX_PORT))
		throw InvalidPortRangeException(str);
	return (static_cast<int>(n));
}

/* This function sets the fd entered as non-blocking using
fcntl() */
void
Server::setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
		throw FcntlException("F_GETFL");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw FcntlException("F_SETFL");
}

std::string
Server::numericCode(int code)
{
	std::string		   s;
	std::ostringstream oss;
	oss << code;
	s = oss.str();
	while (s.size() < 3)
		s = "0" + s;
	return s;
}

std::string
Server::buildReply(int				  code,
				   const std::string& nick,
				   const std::string& msg) const
{
	return (":" + this->_serverName + " " + numericCode(code) + " " + nick
			+ " :" + msg + "\r\n");
}

/* This function send  reply to the client when a user is done
registering */
/*
void
Server::sendWelcomeBurst(const Client& client) const
{
	std::string replies[5];
	std::string nickname = client.getNickname();

	// 001 RPL_WELCOME
	replies[0] = buildReply(
		1, nickname, "Welcome to the IRC Network " + client.getPrefix());

	// 002 RPL_YOURHOST
	replies[1] = buildReply(2,
							nickname,
							"Your host is " + this->_serverName
								+ ", running version 1.0");

	// 003 RPL_CREATED
	time_t now = time(NULL);
	char   dateBuf[64];
	strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", localtime(&now));
	replies[2] = buildReply(
		3, nickname, std::string("This server was created ") + dateBuf);

	// 004 RPL_MYINFO — no trailing colon
	replies[3] = ":" + this->_serverName + " 004 " + nickname + " "
				 + this->_serverName + " 1.0 io itkol\r\n";

	// 422 ERR_NOMOTD — required by irssi to unlock the prompt
	replies[4] = buildReply(422, nickname, "MOTD File is missing");

	for (int i = 0; i < 5; i++)
	{
		if (send(client.getFd(), replies[i].c_str(), replies[i].size(), 0)
			== -1)
		{
			std::cerr << "send() failed on welcome burst reply " << i
					  << std::endl;
			return;
		}
	}
}

:ft_irc.42lyon.fr 001 t :Welcome to the IRC Network t!t@127.0.0.1
:ft_irc.42lyon.fr 002 t :Your host is ft_irc.42lyon.fr, running version 1.0
:ft_irc.42lyon.fr 003 t :This server was created 2026-04-30
:ft_irc.42lyon.fr 004 t ft_irc.42lyon.fr 1.0 io itkol
:ft_irc.42lyon.fr 422 t :MOTD File is missing

*/

void
Server::sendWelcomeBurst(const Client& client)
{
	ServerReply::reply(client, ServerReply::RPL_WELCOME);
	ServerReply::reply(client, ServerReply::RPL_YOURHOST);
	ServerReply::reply(client, ServerReply::RPL_CREATED);
	ServerReply::reply(client, ServerReply::RPL_MYINFO);
	ServerReply::reply(client, ServerReply::ERR_NOMOTD);
}

/* This function adds a new client not known by the server
using the epoll_ctl() with flag EPOLL_CTL_ADD */
void
Server::addNewClient(void)
{
	int				   clientFd;
	std::string		   clientHostname;
	struct epoll_event ev;

	this->acceptClient(clientFd, clientHostname);

	if (clientFd < 0)
		return;

	setNonBlocking(clientFd);

	memset(&ev, 0, sizeof(ev));
	ev.events  = EPOLLIN;
	ev.data.fd = clientFd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, clientFd, &ev) < 0)
		throw EpollCtlException("EPOLL_CTL_ADD");

	this->_clients.push_back(new Client(clientFd, clientHostname));
}

/* This function returns the read status depending on the number
of bytes read in the fd and the value of errno */
Server::ReadStatus
Server::getReadStatus(int fd, char* buffer, ssize_t& n)
{
	n = recv(fd, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT);
	if (n < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (READ_AGAIN);
		return (READ_ERROR);
	}
	if (n == 0)
		return (READ_DISCONNECT);
	return (READ_OK);
}

/* This function removes a client from epoll, closes its fd,
and deletes it from the clients list */
bool
Server::removeClient(int fd)
{
	std::vector<Client*>::iterator it =
		std::find_if(this->_clients.begin(),
					 this->_clients.end(),
					 utils::HasMemberValue<Client, int>(&Client::getFd, fd));

	if (it == _clients.end())
		return (false);

	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0)
		throw EpollCtlException("EPOLL_CTL_DEL");
	shutdown(fd, SHUT_RDWR);
	close(fd);
	delete (*it);
	this->_clients.erase(it);

	return (true);
}

Client*
Server::getClientByFd(const int fd) const
{
	std::vector<Client*>::const_iterator it =
		std::find_if(this->_clients.begin(),
					 this->_clients.end(),
					 utils::HasMemberValue<Client, int>(&Client::getFd, fd));
	if (it == this->_clients.end())
		return (NULL);
	return (*it);
}

Client*
Server::getClientByNick(const std::string& nick) const
{
	std::vector<Client*>::const_iterator it = std::find_if(
		this->_clients.begin(),
		this->_clients.end(),
		utils::HasMemberValue<Client, std::string>(&Client::getNickname, nick));
	if (it == this->_clients.end())
		return (NULL);
	return (*it);
}

Channel*
Server::getChannelByName(const std::string& name) const
{
	std::vector<Channel*>::const_iterator it = std::find_if(
		this->_channels.begin(),
		this->_channels.end(),
		utils::HasMemberValue<Channel, std::string>(&Channel::getName, name));
	if (it == this->_channels.end())
		return (NULL);
	return (*it);
}

Channel*
Server::getOrCreateChannel(const std::string& name)
{
	Channel* channel;

	channel = this->getChannelByName(name);
	if (channel == NULL)
	{
		channel = new Channel(name);
		this->_channels.push_back(channel);
	}
	return (channel);
}

/* This function handles all the events received in a loop iterating
over all the events received and deciding logic to adapt according
to what it has received */
void
Server::handleEvents(struct epoll_event events[MAX_EVENTS], int nfds)
{
	for (int i = 0; i < nfds; ++i)
	{
		if (events[i].data.fd == this->_listen_sock)
			addNewClient();
		else
		{
			int		fd = events[i].data.fd;
			char	buffer[BUFFER_SIZE];
			ssize_t n;

			ReadStatus status = this->getReadStatus(fd, buffer, n);

			if (status == READ_AGAIN)
				continue;
			if (status == READ_ERROR || status == READ_DISCONNECT)
			{
				removeClient(fd);
				continue;
			}

			Client* client = this->getClientByFd(fd);
			if (client == NULL)
				continue;

			client->appendToBuffer(std::string(buffer, n));

			std::vector<std::string> messages = client->extractMessages();

			CommandDispatcher disp;
			CommandParser	  parser(disp);
			try
			{
				for (size_t j = 0; j < messages.size(); j++)
					parser.parse(*client, messages[j]);
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}
}

/* This function initializes epoll, registers the listening socket,
and runs the main event loop */
void
Server::start(void)
{
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];

	this->_epoll_fd = epoll_create1(0);
	if (this->_epoll_fd < 0)
		throw EpollCreateException();

	setNonBlocking(this->_listen_sock);

	memset(&ev, 0, sizeof(ev));
	ev.events  = EPOLLIN;
	ev.data.fd = this->_listen_sock;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_listen_sock, &ev) < 0)
		throw EpollCtlException("EPOLL_CTL_ADD");

	while (g_running)
	{
		int nfds = epoll_wait(this->_epoll_fd, events, MAX_EVENTS, -1);
		if (nfds < 0)
		{
			if (errno == EINTR)
				continue;
			throw EpollWaitException();
		}
		handleEvents(events, nfds);
	}
}

bool
Server::isPasswordValid(const std::string& password)
{
	return (password == this->_password);
}

std::string
Server::getServerName(void) const
{
	return (this->_serverName);
}

std::string
Server::getCreationDate(void) const
{
	return (this->_creationDate);
}
