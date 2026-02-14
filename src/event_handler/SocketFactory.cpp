/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketFactory.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 18:24:46 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/12 18:24:47 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketFactory.hpp"

SocketFactory::SocketException::SocketException(const std::string &msg): _msg(msg){}
SocketFactory::SocketException::SocketException(const char *msg): _msg(msg){}
SocketFactory::SocketException::~SocketException() throw(){}
const char	*SocketFactory::SocketException::what() const throw()
{
	return (_msg.c_str());
}

void	SocketFactory::create(const std::vector<ServerConfig> &servers)
{
	try
	{
		for (std::vector<ServerConfig>::const_iterator it = servers.begin(); it != servers.end(); it++)
		{
			createServerSockets(*it);
		}
	}
	catch (const std::exception &e)
	{
		cleanup();
		throw ;
	}
}

void	SocketFactory::createServerSockets(const ServerConfig &server)
{
	const std::vector<ListenConfig>	&listens = server.getListens();

	for (std::vector<ListenConfig>::const_iterator it = listens.begin(); it != listens.end(); it++)
	{
		const ListenConfig	&cfg = *it;
		struct addrinfo		hints;
		struct addrinfo		*result = NULL;

		std::memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;			// IPv4
		hints.ai_socktype = SOCK_STREAM;	// TCP
		hints.ai_flags = AI_PASSIVE;		// BIND

		std::ostringstream	portStream;
		portStream << cfg.port;
		std::string	portStr = portStream.str();

		int status = getaddrinfo(cfg.host.c_str(), portStr.c_str(), &hints, &result);
		if (status != 0)
			throw (SocketException(gai_strerror(status)));
		int	fd = -1;
		struct addrinfo	*p;

		for (p = result; p != NULL; p = p->ai_next)
		{
			fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
			if (fd < 0)
				continue ;
			int optval = 1;
			if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
			{
				close(fd);
				fd = -1;
				continue ;
			}
			if (bind(fd, p->ai_addr, p->ai_addrlen) == 0)
				break ;
			close(fd);
			fd = -1;
		}
		freeaddrinfo(result);
		if (fd < 0)
			throw (SocketException("failed to bind any address for server."));
		if (listen(fd, SOMAXCONN) < 0)
		{
			close(fd);
			throw (SocketException("failed to listen on socket."));
		}

		ListenSocket	*listener = new ListenSocket(fd, &server);
		EventHandler::setNonBlocking(listener);
		_listeners.push_back(listener);
	}
}

void	SocketFactory::cleanup(void)
{
	for (std::vector<Socket*>::iterator it = _listeners.begin(); it != _listeners.end(); it++)
	{
		if (*it)
		{
			close(((*it)->getFd()));
			(*it)->setFd(-1);
			delete (*it);
		}
	}
	_listeners.clear();
}

const std::vector<Socket*>	&SocketFactory::getListeners(void) const
{
	return (_listeners);
}

SocketFactory::SocketFactory()
{

}

SocketFactory::~SocketFactory()
{

}