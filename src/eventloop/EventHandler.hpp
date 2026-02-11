/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:47:03 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/11 17:47:03 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER
#define EVENTHANDLER

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include "Config.hpp"

typedef int	fd_t;

enum class SocketType
{
	LISTENER,
	CLIENT
};

class Socket
{
	private:
		SocketType	_type;
		pollfd		_prq;
	public:
		Socket(fd_t fd, SocketType type);
		virtual ~Socket();
		virtual void updatePollEvents() = 0;
	public:
		SocketType	getType(void) const;
	public:
		fd_t		getFd(void) const;
		short		getEvents(void) const;
		short		getRevents(void) const;
		pollfd		&getPollFd(void);
	public:
		void		setEvents(short events);
		void		setRevents(short events);
		void		setFd(fd_t fd);	
	public:
		void		addEvent(short event);
		void		removeEvent(short event);
	public:
		bool		hasRevent(short event) const;
};

class Client : public Socket
{
	private:
		std::string		_writeBuffer;
		std::string		_readBuffer;
		ServerConfig	*_server;
		bool			_keepAlive;
		size_t			_writePos;
	public:
		Client(fd_t fd);
		~Client();
	public:
		const std::string	&getWriteBuffer(void) const;
		const std::string	&getReadBuffer(void) const;
		ServerConfig		*getServer(void) const;
		void				setServer(ServerConfig *srv);
		bool				hasDataToSend(void) const;
		bool				getKeepAlive(void) const;
		void				setKeepAlive(bool val);
		size_t				&getWritePos(void);
		void				updatePollEvents(void);
		void				appendToReadBuffer(char	*buf, ssize_t n);
};

class Listener : public Socket
{
	public:
		Listener(fd_t fd);
		~Listener();
	public:
		void	updatePollEvents(void);
};

class EventHandler
{
	private:
		std::vector<Socket*>	_sockets;
		static void	setNonBlocking(Socket *sock);
		void		acceptClient(Socket *sock);
		void		readClient(Client *sock);
		void		writeClient(Client *sock);
	public:
		void		run(void);
		void		addSocket(Socket *sock);
		EventHandler();
		~EventHandler();
};


#endif