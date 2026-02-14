/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:26:11 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/12 17:26:12 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTEN_SOCKET
#define LISTEN_SOCKET

#include "Socket.hpp"
#include "Config.hpp"

class ListenSocket : public Socket
{
	public:
		ListenSocket(fd_t fd, const ServerConfig *srv);
		~ListenSocket();
	public:
		void	updatePollEvents(void);
};

#endif