/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 23:05:18 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/10 23:05:19 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <string>
# include <vector>
# include <map>

class ConfigParser
{
	private:
		enum ParseState
		{
			GLOBAL_BLOCK,
			SERVER_BLOCK,
			LOCATION_BLOCK
		};

		struct LocationConfig
		{
			/*
			location
				├── methods
				├── root
				├── index
				├── autoindex
				├── upload_path
				├── return
				└── cgi
			*/
			std::string	path;
			std::string	root;
			std::string	index;
			std::string	upload_path;
			std::string	redirect_target;
			std::string	cgi_extension;
			std::string	cgi_path;
			bool	autoindex;
			bool	upload_enabled;
			bool	has_redirect;
			bool	cgi_enabled;
			int		redirect_code;
			std::vector<std::string>	methods;
		};

		struct ServerConfig
		{
			/*
			server
				├── listen
				├── error_page
				├── client_max_body_size
				└── location
			*/
			std::vector<int>	ports;
			std::vector<std::string>	hosts;
			std::vector<LocationConfig>	locations;
			std::map<int, std::string>	error_pages;
			size_t	client_max_body_size;
		};
	public:
		ConfigParser();
		ConfigParser(std::string file_path);
		~ConfigParser();
};

#endif