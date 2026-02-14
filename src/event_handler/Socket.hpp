/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:17:18 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/12 17:17:19 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET
#define SOCKET

#include <poll.h>
#include <stdlib.h>
#include "Config.hpp"
#include <unistd.h>

typedef int	fd_t;

enum SocketType
{
	LISTEN,
	CLIENT,
	SIGNAL
};

class Socket
{
	private:
		SocketType			_type;
		pollfd				_prq;
		bool				_markedForDisconnect;
		const ServerConfig	*_server;
	public:
		Socket(fd_t fd, SocketType type, const ServerConfig *srv);
		virtual ~Socket();
		virtual void updatePollEvents() = 0;
	public:
		SocketType	getType(void) const;
	public:
		fd_t		getFd(void) const;
		short		getEvents(void) const;
		short		getRevents(void) const;
		pollfd		&getPollFd(void);
		const ServerConfig	*getServer(void) const;
	public:
		void		setEvents(short events);
		void		setRevents(short events);
		void		setFd(fd_t fd);
		void		closeFd(void);
	public:
		void		addEvent(short event);
		void		removeEvent(short event);
	public:
		bool		hasRevent(short event) const;
		bool		isMarked(void);
		void		setMarked(bool mark);
};

#endif