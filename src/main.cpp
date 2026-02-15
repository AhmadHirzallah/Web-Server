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
#include "EventHandler.hpp"
#include "SocketFactory.hpp"

int	g_signal_write_fd = -1;

int main()
{
	Config					cnf("server.conf");
	EventHandler			handleer;
	SocketFactory			socketeer;
	std::vector<Socket*>	listenSockets;

	try
	{
		cnf.parse(); // throws Config::ParsingException
		socketeer.create(cnf.getServers());
		handleer.addSocket(socketeer.getListeners());
		handleer.run(); // throws EventHandler::EventException
	}
	catch (const Config::ParsingException &e)
	{
		std::cerr << "webserv: " << e.what() << std::endl;
		return (1);
	}
	catch (const EventHandler::EventException &e)
	{
		std::cerr << "webserv: " << e.what() << std::endl;
		handleer.cleanup();
		return (2);
	}
}
