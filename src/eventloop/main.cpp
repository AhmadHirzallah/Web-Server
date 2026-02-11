/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:44:17 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/09 15:44:17 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventLoop.hpp"

int createListener(int port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	setNonBlocking(fd);

	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	bind(fd, (sockaddr*)&addr, sizeof(addr));
	listen(fd, 128);

	return (fd);
}
// Prepare (Listen Sockets) -> Enter Listeners -> Ready
int main()
{
	EventLoop loop;

	int listenFd = createListener(8080);
	loop.addListener(listenFd);

	loop.run();
}