/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:17:23 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/12 17:17:24 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

fd_t	Socket::getFd(void) const
{
	return (_prq.fd);
}

SocketType	Socket::getType(void) const
{
	return (_type);
}

Socket::Socket(fd_t fd, SocketType type, const ServerConfig *srv): _type(type), _markedForDisconnect(false), _server(srv)
{
	_prq.fd = fd;
	_prq.events = 0;
	_prq.revents = 0;
}

const ServerConfig	*Socket::getServer(void) const
{
	return (_server);
}

void	Socket::setEvents(short event)
{
	_prq.events = event;
}

void	Socket::setRevents(short event)
{
	_prq.revents = event;
}

void	Socket::setFd(fd_t fd)
{
	_prq.fd = fd;
}

short	Socket::getEvents(void) const
{
	return (_prq.events);
}

short	Socket::getRevents(void) const
{
	return (_prq.revents);
}

void	Socket::addEvent(short event)
{
	_prq.events |= event;
}

void	Socket::removeEvent(short event)
{
	_prq.events &= ~event;
}

bool	Socket::hasRevent(short event) const
{
	return ((_prq.revents & event) != 0);
}

pollfd		&Socket::getPollFd(void)
{
	return (_prq);
}

bool		Socket::isMarked(void)
{
	return (_markedForDisconnect);
}

void		Socket::setMarked(bool mark)
{
	_markedForDisconnect = mark;
}

void	Socket::closeFd(void)
{
	close(getFd());
}

Socket::~Socket()
{

}
