/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:59:11 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/29 15:59:11 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

LocationConfig::LocationConfig(const LocationData &cfg): data(cfg){}
LocationConfig::~LocationConfig(){}

const std::string&	LocationConfig::getPath(void) const {return (this->data.path);}
const std::string&	LocationConfig::getRoot(void) const {return (this->data.root);}
const std::string&	LocationConfig::getIndex(void) const {return (this->data.index);}
const std::string&	LocationConfig::getCgiExtension(void) const {return (this->data.cgi_extension);}
const std::vector<std::string>&	LocationConfig::getMethods(void) const {return (this->data.methods);}
const std::string&	LocationConfig::getCgiPath(void) const {return (this->data.cgi_path);}
const std::string&	LocationConfig::getUploadPath(void) const {return (this->data.upload_path);}
const std::string&	LocationConfig::getRedirectTarget(void) const {return (this->data.redirect_target);}
int		LocationConfig::getRedirectCode(void) const {return (this->data.redirect_code);}
bool	LocationConfig::autoIndexEnabled(void) const {return (this->data.autoindex);}
bool	LocationConfig::uploadEnabled(void) const {return (this->data.upload_enabled);}
bool	LocationConfig::redirectEnabled(void) const {return (this->data.has_redirect);}
bool	LocationConfig::cgiEnabled(void) const {return (this->data.cgi_enabled);}