/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:22:51 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/12 17:22:52 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_SOCKET
#define CLIENT_SOCKET

#include "Config.hpp"
#include "Socket.hpp"
#include <string>
#include <iostream>
#include <vector>

class ClientSocket : public Socket
{
	private:
		std::string		_writeBuffer;
		std::string		_readBuffer;
		bool			_keepAlive;
	public:
		ClientSocket(fd_t fd, const ServerConfig *srv);
		~ClientSocket();
	public:
		const std::string	&getWriteBuffer(void) const;
		const std::string	&getReadBuffer(void) const;
		bool				hasDataToSend(void) const;
		bool				getKeepAlive(void) const;
		void				setKeepAlive(bool val);
		void				updatePollEvents(void);
		void				appendToReadBuffer(const char *data, ssize_t n);
		void				appendToWriteBuffer(const char *data, ssize_t n);
		// bool				hasCompleteRequest() const;
		void				setWriteBuffer(const std::string &data);
		void				clearReadBuffer(void);
		void				clearWriteBuffer(void);
		void				eraseWriteBuffer(size_t n);
};

#endif
