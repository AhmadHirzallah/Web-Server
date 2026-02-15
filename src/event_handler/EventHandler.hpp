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
#include <signal.h>

extern int	g_signal_write_fd;

class EventHandler
{
	private:
		std::vector<Socket*>	_sockets;
		std::vector<Socket*>	_disconnected;
		bool					_running;
		bool					_shuttingDown;
		void		acceptClient(Socket *sock);
		void		readClient(ClientSocket *sock);
		void		writeClient(ClientSocket *sock);
		void		disconnectSocket(Socket *sock);
		void		markDisconnected(Socket *sock);
		void		initSignalSocket(void);
		void		initSignalHandler(void);
		void		signalHandle(Socket *sock);
	public:
		void		run(void);
		static void	setNonBlocking(Socket *sock);
		static void	setNonBlocking(fd_t fd);
		void		addSocket(Socket *sock);
		void		addSocket(std::vector<Socket*> socks);
		void		cleanup(void);
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