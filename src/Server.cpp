/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:52:35 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/27 18:26:03 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "CommandDispatcher.hpp"
#include "CommandParser.hpp"
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
	_epoll_fd(-1),
	_listen_sock(-1),
	_port(port),
	_password(password),
	_clients()
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
			return ; // no client actually ready, not a real error
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

/* This function adds a new client not known by the server
using the epoll_ctl() with flag EPOLL_CTL_ADD */
void
Server::addNewClient(void)
{
	int				 	clientFd;
	std::string			clientHostname;
	struct epoll_event	ev;

	this->acceptClient(clientFd, clientHostname);

	if (clientFd < 0)
		return ;

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
			std::vector<Client*>::iterator it = std::find_if(
				this->_clients.begin(),
				this->_clients.end(),
				utils::HasMemberValue<Client, int>(&Client::getFd, fd));

			if (it == this->_clients.end())
				continue;

			(*it)->appendToBuffer(std::string(buffer, n));

			std::vector<std::string> messages = (*it)->extractMessages();

			CommandDispatcher disp;
			CommandParser	  parser(disp);
			try
			{
				for (size_t j = 0; j < messages.size(); j++)
					parser.parse(**it, messages[j]);
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

	while (true)
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
