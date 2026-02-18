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

EventHandler::EventException::EventException(const std::string &msg): _msg(msg){}
EventHandler::EventException::EventException(const char *msg): _msg(msg){}
EventHandler::EventException::~EventException() throw(){}
const char	*EventHandler::EventException::what() const throw()
{
	return (_msg.c_str());
}

void	EventHandler::run(void)
{
	if (_sockets.empty())
		throw (EventException("no listening sockets!"));
	initSignalHandler();
	initSignalSocket();
	while (_running)
	{
		std::vector<pollfd>	pfds;
		pfds.reserve(_sockets.size());

		for (std::size_t i = 0; i < _sockets.size(); ++i)
		{
			Socket	*sock = _sockets[i];

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

		for (std::size_t i = 0; i < _sockets.size(); ++i)
		{
			Socket	*sock = _sockets[i];

			if (sock->getRevents() == 0)
				continue ;
			if (sock->hasRevent(POLLERR | POLLNVAL))
			{
				markDisconnected(sock);
				continue ;
			}
			if (sock->getType() == SIGNAL && sock->hasRevent(POLLIN))
			{
				signalHandle(sock);
				return ;
			}
			if (sock->getType() == LISTEN && sock->hasRevent(POLLIN))
			{
				if (!_shuttingDown)
					acceptClient(dynamic_cast<ListenSocket*>(sock));
				continue ;
			}
			if (sock->getType() == CLIENT)
			{
				if (sock->hasRevent(POLLIN))
					readClient(dynamic_cast<ClientSocket*>(sock));
				if (sock->hasRevent(POLLOUT))
					writeClient(dynamic_cast<ClientSocket*>(sock));
			}
		}

		if (!_disconnected.empty())
		{
			for (std::vector<Socket*>::iterator dit = _disconnected.begin();
				dit != _disconnected.end(); dit++)
			{
				Socket	*dead = *dit;

				if (!dead)
					continue ;
				std::cout << "Client disconnected: " << dead->getFd() << std::endl;
				disconnectSocket(dead);
			}
			_disconnected.clear();
		}
	}
}

void	EventHandler::disconnectSocket(Socket *sock)
{
	_sockets.erase(std::remove(_sockets.begin(), _sockets.end(), sock), _sockets.end());
	close(sock->getFd());
	delete (sock);
}

void	EventHandler::readClient(ClientSocket *sock)
{
	char	buf[4096];
	ssize_t	n = recv(sock->getFd(), buf, sizeof(buf), 0);

	if (n > 0)
	{
		sock->appendToWriteBuffer(buf, n); //httpParse
	}
	else
	{
		markDisconnected(sock);
	}
}

void	EventHandler::writeClient(ClientSocket *sock)
{
	ssize_t	n = send(sock->getFd(), sock->getWriteBuffer().data(), sock->getWriteBuffer().size(), 0);

	if (n > 0)
		sock->eraseWriteBuffer(n);
	else
	{
		markDisconnected(sock);
		return ;
	}
	if (sock->getWriteBuffer().empty())
	{
		sock->removeEvent(POLLOUT);
	}
}

void	EventHandler::markDisconnected(Socket *sock)
{
	if (sock->isMarked())
		return ;
	sock->setMarked(true);
	_disconnected.push_back(sock);
}

void	EventHandler::acceptClient(Socket *sock)
{
	while (true)
	{
		int	clientFd = accept(sock->getFd(), NULL, NULL);

		if (clientFd < 0)
			break ;
		ClientSocket	*c = new ClientSocket(clientFd, sock->getServer());
		setNonBlocking(c);
		_sockets.push_back(c);
		std::cout << "Client connected: " << c->getFd() << std::endl;
	}
}

void	EventHandler::addSocket(Socket *sock)
{
	_sockets.push_back(sock);
}

void	EventHandler::setNonBlocking(Socket *sock)
{
	int	flags = fcntl(sock->getFd(), F_GETFL, 0);
	fcntl(sock->getFd(), F_SETFL, flags | O_NONBLOCK);
}

void		EventHandler::cleanup(void)
{
	for (std::vector<Socket*>::const_iterator it = _sockets.begin(); it != _sockets.end(); it++)
	{
		(*it)->closeFd();
		(*it)->setFd(-1);
		delete (*it);
	}
	if (g_signal_write_fd != -1)
		close(g_signal_write_fd);
	_sockets.clear();
}

EventHandler::EventHandler(): _running(true), _shuttingDown(false)
{

}

EventHandler::~EventHandler()
{
	
}

void		EventHandler::signalHandle(Socket *sock)
{
	_shuttingDown = true;
	char	buffer[16];
	read(sock->getFd(), buffer, sizeof(buffer));
	_running = false;
	cleanup();
	close(g_signal_write_fd);
}

void	EventHandler::setNonBlocking(fd_t fd)
{
	int	flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void	EventHandler::initSignalSocket(void)
{
	int	pipefd[2];
	if (pipe(pipefd) < 0)
		throw (EventException("failed to create pipe for SignalSocket."));
	g_signal_write_fd = pipefd[1];
	setNonBlocking(pipefd[0]);
	addSocket(new SignalSocket(pipefd[0]));
}

void	handle_signal(int sig)
{
	(void)sig;
	char	byte = 1;
	write(g_signal_write_fd, &byte, 1);
}

void		EventHandler::addSocket(std::vector<Socket*> socks)
{
	for (std::size_t i = 0; i < socks.size(); i++)
		addSocket(socks[i]);
}

void		EventHandler::initSignalHandler(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
	signal(SIGQUIT, handle_signal);
}
