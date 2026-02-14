/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 16:12:08 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/29 16:12:09 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

ServerConfig::ServerConfig(const ServerData &cfg): data(cfg){}
ServerConfig::~ServerConfig(){}

const std::vector<LocationConfig>	&ServerConfig::getLocations(void) const {return (this->data.locations);}
const std::vector<ListenConfig>		&ServerConfig::getListens(void) const {return (this->data.listens);}
size_t	ServerConfig::getClientMaxBodySize(void) const {return (this->data.client_max_body_size);}
const std::map<int, std::string> &ServerConfig::getErrorPages(void) const {return (this->data.error_pages);}