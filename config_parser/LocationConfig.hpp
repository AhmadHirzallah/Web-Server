/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 19:20:20 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/16 19:20:20 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP

# include <string>
# include <vector>
# include <iostream>

struct LocationConfigData
{
	std::string	path;
	std::string	root;
	std::string	index;
	bool	autoindex;
	std::vector<std::string>	methods;
	bool	upload_enabled;
	std::string	upload_path;
	bool	has_redirect;
	int		redirect_code;
	std::string	redirect_target;
	bool	cgi_enabled;
	std::string	cgi_extension;
	std::string	cgi_path;

	LocationConfigData(): autoindex(false),
							upload_enabled(false),
							has_redirect(false),
							redirect_code(0),
							cgi_enabled(false){}
};

class LocationConfig
{
	/*Private Attributes*/
	private:
		LocationConfigData	data;
	/*Public Methods*/
	public:
		const std::string&	getPath(void) const;
		const std::string&	getRoot(void) const;
		const std::string&	getIndex(void) const;
		const std::string&	getCgiExtension(void) const;
		const std::string&	getCgiPath(void) const;
		const std::string&	getUploadPath(void) const;
		const std::string&	getRedirectTarget(void) const;
		const std::vector<std::string>&	getMethods(void) const;
		int		getRedirectCode(void) const;
		bool	autoIndexEnabled(void) const;
		bool	uploadEnabled(void) const;
		bool	redirectEnabled(void) const;
		bool	cgiEnabled(void) const;
		void	print(void) const;
	/*Constructors & Destructors*/
	public:
		LocationConfig();
		LocationConfig(const LocationConfigData &cfg);
		~LocationConfig();
};

#endif