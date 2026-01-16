/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 21:51:25 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/16 21:51:26 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

const std::vector<LocationConfig>	ServerConfig::getLocations(void) const
{
	return (this->data.locations);
}

size_t	ServerConfig::getClientMaxBodySize(void) const
{
	return (this->data.client_max_body_size);
}

const std::map<int, std::string> ServerConfig::getErrorPages(void) const
{
	return (this->data.error_pages);
}

const std::vector<int>	ServerConfig::getPorts(void) const
{
	return (this->data.ports);
}

const std::vector<std::string>&	ServerConfig::getHosts(void) const
{
	return (this->data.hosts);
}

ServerConfig::ServerConfig(): data(){}
ServerConfig::ServerConfig(const ServerConfigData &cfg): data(cfg){}
ServerConfig::~ServerConfig(){}

void	ServerConfig::print(void) const
{
	std::cout << "- Server in Mem: " << this << std::endl;
	std::cout << "Client Max Body Size: " << getClientMaxBodySize() << std::endl;
	for (size_t i = 0; i < data.hosts.size(); i++)
	{
		std::cout << data.hosts[i] << " ";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < data.ports.size(); i++)
	{
		std::cout << data.ports[i] << " ";
	}
	std::cout << std::endl;
	std::map<int, std::string>::const_iterator it;
	for (it = data.error_pages.begin(); it != data.error_pages.end(); it++)
	{
		std::cout << "Error Page: " << it->first << " => " << it->second << std::endl;
	}
	for (size_t i = 0; i < data.locations.size(); i++)
	{
		data.locations[i].print();
	}
}
