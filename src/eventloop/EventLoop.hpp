/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:52:59 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/09 15:53:00 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTLOOP
#define EVENTLOOP

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

struct Client
{
	int			fd;
	std::string	readBuffer;
	std::string	writeBuffer;
};

class EventLoop
{
	private:
		std::vector<pollfd>		_pfds;
		std::set<int>			_listeners;
		std::map<int, Client>	_clients;
	public:
		EventLoop();
		~EventLoop();
		void	addListener(int fd);
		void	run(void);
	private:
		void	handleEvent(pollfd &p);
		void	acceptClient(int listenFd);
		void	readClient(int clientFd);
		void	writeClient(int clientFd);
};

void	setNonBlocking(int fd);

#endif