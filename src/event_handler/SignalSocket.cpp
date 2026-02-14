/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:39:34 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/14 17:39:35 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SignalSocket.hpp"

SignalSocket::SignalSocket(fd_t fd): Socket(fd, SIGNAL, NULL)
{
	setEvents(POLLIN);	
}

SignalSocket::~SignalSocket()
{

}

void	SignalSocket::updatePollEvents(void)
{

}

void	SignalSocket::clearSignal(void)
{
	char	buf[10];
	read(getFd(), buf, sizeof(buf));
}
