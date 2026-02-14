/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:39:30 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/14 17:39:30 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_SOCKET
#define SIGNAL_SOCKET

#include "Socket.hpp"

class SignalSocket : public Socket
{
	private:
		/* data */
	public:
		SignalSocket(fd_t fd);
		~SignalSocket();
		void	updatePollEvents(void);
		void	clearSignal(void);
};

#endif
