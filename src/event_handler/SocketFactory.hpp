/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketFactory.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 18:24:41 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/12 18:24:42 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_FACTORY
#define SOCKET_FACTORY

#include <vector>
#include "Socket.hpp"
#include "EventHandler.hpp"
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class SocketFactory
{
	private:
		std::vector<Socket*>	_listeners;
		void	cleanup(void);
		void	createServerSockets(const ServerConfig &server);
	public:
		void	create(const std::vector<ServerConfig> &servers);
		const std::vector<Socket*>	&getListeners(void) const;
		SocketFactory();
		~SocketFactory();

		class SocketException: public std::exception
		{
			private:
				std::string	_msg;
			public:
				explicit SocketException(const std::string &msg);
				explicit SocketException(const char *msg);
				virtual ~SocketException() throw();
				virtual const char	*what() const throw();
		};
};

#endif