/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 21:51:18 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/16 21:51:19 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include "LocationConfig.hpp"

struct ServerConfigData
{
	std::vector<std::string>	hosts;
	std::vector<int>	ports;
	size_t	client_max_body_size;
	std::map<int, std::string>	error_pages;
	std::vector<LocationConfig>	locations;

	ServerConfigData(): client_max_body_size(1024*1024){}
};

class ServerConfig
{
	/*Private Attributes*/
	private:
		ServerConfigData	data;
	/*Public Methods*/
	public:
		const std::vector<LocationConfig>	getLocations(void) const;
		size_t getClientMaxBodySize(void) const;
		const std::map<int, std::string> getErrorPages(void) const;
		const std::vector<int>	getPorts(void) const;
		const std::vector<std::string>&	getHosts(void) const;
		void print(void) const;
	/*Constructor & Destructor*/
	public:
		ServerConfig();
		ServerConfig(const ServerConfigData &cfg);
		~ServerConfig();
};

#endif