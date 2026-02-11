/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:53:07 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/09 15:53:08 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventLoop.hpp"

EventLoop::EventLoop()
{
}

EventLoop::~EventLoop()
{
}

void	EventLoop::addListener(int fd)
{
	pollfd	p;
	p.fd = fd;
	p.events = POLLIN;
	p.revents = 0;

	_pfds.push_back(p);
	_listeners.insert(fd);
}

void	EventLoop::run()
{
	while (true)
	{
		int	ready = poll(_pfds.data(), _pfds.size(), -1);

		if (ready <= 0)
			continue ;
		for (size_t i = 0; i < _pfds.size(); i++)
		{
			if (_pfds[i].revents == 0)
				continue ;
			handleEvent(_pfds[i]);
		}
	}
}

void	EventLoop::handleEvent(pollfd &p)
{
	int	fd = p.fd;

	if (_listeners.count(fd))
	{
		acceptClient(fd);
		return ;
	}
	if (p.revents & POLLIN)
		readClient(fd);
	if (p.revents & POLLOUT)
		writeClient(fd);
}

void	setNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void	EventLoop::acceptClient(int listenFd)
{
	while (true)
	{
		int clientFd = accept(listenFd, NULL, NULL);

		if (clientFd < 0)
			break ;
		setNonBlocking(clientFd);

		Client c;
		c.fd = clientFd;
		_clients[clientFd] = c;
		pollfd p;
		p.fd = clientFd;
		p.events = POLLIN;
		p.revents = 0;
		_pfds.push_back(p);
		std::cout << "Client connected: " << clientFd << std::endl;
	}
}

void	EventLoop::readClient(int fd)
{
	char buf[4096];
	ssize_t n = recv(fd, buf, sizeof(buf), 0);

	if (n > 0)
	{
		Client &c = _clients[fd];
		c.readBuffer.append(buf, n);
		c.writeBuffer += c.readBuffer;
		c.readBuffer.clear();
		for (size_t i = 0; i < _pfds.size(); i++)
			if (_pfds[i].fd == fd)
				_pfds[i].events |= POLLOUT;
	}
	else{
		close (fd);
		_clients.erase(fd);
		std::cout << "Client disconnected\n";
	}
}

void	EventLoop::writeClient(int fd)
{
	Client &c = _clients[fd];

	ssize_t n = send(fd, c.writeBuffer.data(), c.writeBuffer.size(), 0);

	if (n > 0)
		c.writeBuffer.erase(0, n);
	else
	{
		close(fd);
		_clients.erase(fd);
		return ;
	}
	if (c.writeBuffer.empty())
	{
		for (size_t i = 0; i < _pfds.size(); i++)
			if (_pfds[i].fd == fd)
				_pfds[i].events &= ~POLLOUT;
	}
}