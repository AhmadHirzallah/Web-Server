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

ServerConfig::ServerConfig(): data(){}
ServerConfig::ServerConfig(const ServerConfigData &cfg): data(cfg){}
ServerConfig::~ServerConfig(){}

void	ServerConfig::print(void) const
{
	std::cout << "- Server in Mem: " << this << std::endl;
	std::cout << "Client Max Body Size: " << getClientMaxBodySize() << std::endl;
	for (size_t i = 0; i < data.listens.size(); i++)
	{
		std::cout << "Host: '" << data.listens[i].host << "' Port: '" << data.listens[i].port << "'" << std::endl;	
	}
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
