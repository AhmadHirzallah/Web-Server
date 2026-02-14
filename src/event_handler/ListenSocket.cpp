/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:26:17 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/12 17:26:18 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListenSocket.hpp"

ListenSocket::ListenSocket(fd_t fd, const ServerConfig *srv): Socket(fd, LISTEN, srv)
{
	setEvents(POLLIN);
}

ListenSocket::~ListenSocket()
{

}

void	ListenSocket::updatePollEvents(void)
{
	setEvents(POLLIN);
}
