/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:24:04 by drahwanj          #+#    #+#             */
/*   Updated: 2026/02/12 17:24:05 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

ClientSocket::ClientSocket(fd_t fd, const ServerConfig *srv): Socket(fd, CLIENT, srv), _keepAlive(false)
{
	this->setEvents(POLLIN);
}

ClientSocket::~ClientSocket()
{

}

const std::string	&ClientSocket::getWriteBuffer(void) const
{
	return (this->_writeBuffer);
}

const std::string	&ClientSocket::getReadBuffer(void) const
{
	return (this->_readBuffer);
}

bool	ClientSocket::hasDataToSend(void) const
{
	return (!_writeBuffer.empty());
}

bool	ClientSocket::getKeepAlive(void) const
{
	return (this->_keepAlive);
}

void	ClientSocket::setKeepAlive(bool val)
{
	this->_keepAlive = val;
}

void	ClientSocket::updatePollEvents(void)
{
	if (hasDataToSend())
	{
		addEvent(POLLOUT);
	}
}

void	ClientSocket::clearReadBuffer(void)
{
	_readBuffer.clear();
}

void	ClientSocket::clearWriteBuffer(void)
{
	_writeBuffer.clear();
}

void	ClientSocket::eraseWriteBuffer(size_t n)
{
	if (n >= _writeBuffer.size())
		_writeBuffer.clear();
	else
		_writeBuffer.erase(0, n);
}

void	ClientSocket::appendToReadBuffer(const char *data, ssize_t n)
{
	if (data == NULL || n == 0)
		return ;
	_readBuffer.append(data, n);
}

void	ClientSocket::appendToWriteBuffer(const char *data, ssize_t n)
{
	if (data == NULL || n == 0)
		return ;
	_writeBuffer.append(data, n);
}

void	ClientSocket::setWriteBuffer(const std::string &data)
{
	_writeBuffer = data;
}
