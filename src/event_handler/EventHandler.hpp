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

#ifndef EVENT_HANDLER
#define EVENT_HANDLER

extern int	g_signal_write_fd;

#include "Config.hpp"
#include "Socket.hpp"
#include "ClientSocket.hpp"
#include "ListenSocket.hpp"
#include "SignalSocket.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <algorithm>
#include <cstring>

class EventHandler
{
	private:
		std::vector<Socket*>	_sockets;
		std::vector<Socket*>	_disconnected;
		bool					_running;
		void		acceptClient(Socket *sock);
		void		readClient(ClientSocket *sock);
		void		writeClient(ClientSocket *sock);
		void		disconnectSocket(Socket *sock);
		void		markDisconnected(Socket *sock);
		void		cleanup(void);
		void		initSignalSocket(void);
		void		signalHandle(Socket *sock);
	public:
		void		run(void);
		static void	setNonBlocking(Socket *sock);
		static void	setFdNonBlocking(fd_t fd);
		void		addSocket(Socket *sock);
		EventHandler();
		~EventHandler();
		class EventException: public std::exception
		{
			private:
				std::string	_msg;
			public:
				explicit EventException(const std::string &msg);
				explicit EventException(const char *msg);
				virtual ~EventException() throw();
				virtual const char	*what() const throw();
		};
};

void	handle_signal(int sig);

#endif