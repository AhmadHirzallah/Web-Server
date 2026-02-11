/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:47:10 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/11 17:47:11 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"

/* ************************************************************************** */
/*                                   SOCKET                                   */
/* ************************************************************************** */

fd_t	Socket::getFd(void) const {return (this->_prq.fd);}
SocketType	Socket::getType(void) const {return (this->_type);}
Socket::Socket(fd_t fd, SocketType type): _type(type)
{
	_prq.fd = fd;
	_prq.events = 0;
	_prq.revents = 0;
}
Socket::~Socket() {}
void	Socket::setEvents(short event){this->_prq.events = event;}
void	Socket::setRevents(short event) {this->_prq.revents = event;}
void	Socket::setFd(fd_t fd) {this->_prq.fd = fd;}
short	Socket::getEvents(void) const {return (this->_prq.events);}
short	Socket::getRevents(void) const {return (this->_prq.revents);}
void	Socket::addEvent(short event) {this->_prq.events |= event;}
void	Socket::removeEvent(short event) {this->_prq.events &= ~event;}
bool	Socket::hasRevent(short event) const {return ((this->_prq.revents & event) != 0);}
pollfd		&Socket::getPollFd(void) {return (_prq);}

/* ************************************************************************** */
/*                                   CLIENT                                   */
/* ************************************************************************** */

Client::Client(fd_t fd): Socket(fd, SocketType::CLIENT),
						_server(NULL),
						_keepAlive(false),
						_writePos(0)
						{this->setEvents(POLLIN);}
Client::~Client() {}
const std::string	&Client::getWriteBuffer(void) const {return (this->_writeBuffer);}
const std::string	&Client::getReadBuffer(void) const {return (this->_readBuffer);}
ServerConfig	*Client::getServer(void) const {return (this->_server);}
void	Client::setServer(ServerConfig *srv) {this->_server = srv;}
bool	Client::hasDataToSend(void) const {return (_writePos < _writeBuffer.size());}
bool	Client::getKeepAlive(void) const {return (this->_keepAlive);}
void	Client::setKeepAlive(bool val) {this->_keepAlive = val;}
size_t	&Client::getWritePos(void) {return (this->_writePos);}
void	Client::updatePollEvents(void)
{
	setEvents(POLLIN);
	if (hasDataToSend())
		addEvent(POLLOUT);
}

/* ************************************************************************** */
/*                                 LISTENER                                   */
/* ************************************************************************** */

Listener::Listener(fd_t fd): Socket(fd, SocketType::LISTENER) {setEvents(POLLIN);}
Listener::~Listener() {}
void	Listener::updatePollEvents(void) {setEvents(POLLIN);}

/* ************************************************************************** */
/*                                   MAIN                                     */
/* ************************************************************************** */

void	EventHandler::run(void)
{
	while (true)
	{
		std::vector<pollfd>	pfds;
		pfds.reserve(_sockets.size());

		for (std::vector<Socket*>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
		{
			Socket	*sock = *it;

			sock->updatePollEvents();
			pfds.push_back(sock->getPollFd());
		}

		int ready = poll(pfds.data(), pfds.size(), -1);
		if (ready <= 0)
			continue ;

		for (std::size_t i = 0; i < _sockets.size(); i++)
		{
			_sockets[i]->setRevents(pfds[i].revents);
		}

		for (std::vector<Socket*>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
		{
			Socket	*sock = *it;

			if (sock->getRevents() == 0)
				continue ;
			if (sock->getType() == SocketType::LISTENER && sock->hasRevent(POLLIN))
			{
				acceptClient(sock);
			}
			if (sock->getType() == SocketType::CLIENT && sock->hasRevent(POLLIN))
			{
				readClient(static_cast<Client*>(sock));
			}
			if (sock->getType() == SocketType::CLIENT && sock->hasRevent(POLLOUT))
			{
				writeClient(static_cast<Client*>(sock));
			}
		}
	}
}

void	EventHandler::readClient(Client *sock)
{
	char	buf[4096];
	ssize_t	n = recv(sock->getFd(), buf, sizeof(buf), 0);

	if (n > 0)
	{
		//TODO 
	}
	else
	{
		close(sock->getFd());
	}
}

void	EventHandler::acceptClient(Socket *sock)
{
	while (true)
	{
		int	clientFd = accept(sock->getFd(), NULL, NULL);
		if (clientFd < 0)
			break ;
		setNonBlocking(sock);

		Client	*c = new Client(clientFd);
		_sockets.push_back(c);
		std::cout << "Client connected: " << clientFd << std::endl;
	}
}

void	EventHandler::addSocket(Socket *sock)
{
	this->_sockets.push_back(sock);
}

void	EventHandler::setNonBlocking(Socket *sock)
{
	int	flags = fcntl(sock->getFd(), F_GETFL, 0);
	fcntl(sock->getFd(), F_SETFL, flags | O_NONBLOCK);
}

EventHandler::EventHandler(){}
EventHandler::~EventHandler(){}
