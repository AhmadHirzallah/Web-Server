/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 20:13:56 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/29 20:13:56 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "SocketFactory.hpp"
#include "EventHandler.hpp"
#include <signal.h>

int	g_signal_write_fd = -1;

int main()
{
	Config					cnf("server.conf");
	EventHandler			loop;
	SocketFactory			socketeer;
	std::vector<Socket*>	listenSockets;

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
	signal(SIGQUIT, handle_signal);
	try
	{
		cnf.parse();
		socketeer.create(cnf.getServers());
		listenSockets = socketeer.getListeners();
		for (std::size_t i = 0; i < listenSockets.size(); i++)
		{
			loop.addSocket(listenSockets[i]);
		}
		loop.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << "webserv: " << e.what() << std::endl;
		return (1);
	}
}
