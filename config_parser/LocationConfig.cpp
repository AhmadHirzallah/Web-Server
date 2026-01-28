/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 19:34:07 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/16 19:34:08 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

const std::string&	LocationConfig::getPath(void) const
{
	return (this->data.path);
}

const std::string&	LocationConfig::getRoot(void) const
{
	return (this->data.root);
}

const std::string&	LocationConfig::getIndex(void) const
{
	return (this->data.index);
}

const std::string&	LocationConfig::getCgiExtension(void) const
{
	return (this->data.cgi_extension);
}

const std::vector<std::string>&	LocationConfig::getMethods(void) const
{
	return (this->data.methods);
}

const std::string&	LocationConfig::getCgiPath(void) const
{
	return (this->data.cgi_path);
}

const std::string&	LocationConfig::getUploadPath(void) const
{
	return (this->data.upload_path);
}

const std::string&	LocationConfig::getRedirectTarget(void) const
{
	return (this->data.redirect_target);
}

int	LocationConfig::getRedirectCode(void) const
{
	return (this->data.redirect_code);
}

bool	LocationConfig::autoIndexEnabled(void) const
{
	return (this->data.autoindex);
}

bool	LocationConfig::uploadEnabled(void) const
{
	return (this->data.upload_enabled);
}

bool	LocationConfig::redirectEnabled(void) const
{
	return (this->data.has_redirect);
}

bool	LocationConfig::cgiEnabled(void) const
{
	return (this->data.cgi_enabled);
}

LocationConfig::LocationConfig(): data(){}
LocationConfig::LocationConfig(const LocationConfigData &cfg): data(cfg){}
LocationConfig::~LocationConfig(){}

void	LocationConfig::print() const
{
	std::cout << "- Location in Mem: " << this << std::endl;
	std::cout << "Path: " << getPath() << std::endl;
	std::cout << "Root: " << getRoot() << std::endl;
	std::cout << "Index: " << getIndex() << std::endl;
	std::cout << "AutoIndex: " << autoIndexEnabled() << std::endl;
	std::cout << "UploadEnabled? " << uploadEnabled() << std::endl;
	std::cout << "UploadPath: " << getUploadPath() << std::endl;
	std::cout << "redirectEnabled? " << redirectEnabled() << std::endl;
	std::cout << "redirectCode: " << getRedirectCode() << std::endl;
	std::cout << "redirectTarget: " << getRedirectTarget() << std::endl;
	std::cout << "CGI Enabled? " << cgiEnabled() << std::endl;
	std::cout << "CGI Path: " << getCgiPath() << std::endl;
	std::cout << "CGI Extension: " << getCgiExtension() << std::endl;
	std::cout << "Methods: ";
	for (size_t i = 0; i < data.methods.size(); i++)
	{
		std::cout << data.methods[i] << " ";
	}
	std::cout << std::endl;
}