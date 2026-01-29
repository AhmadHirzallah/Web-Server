/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:44:43 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/29 15:44:44 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG
#define CONFIG

#include <cstdlib>
#include <cerrno>
#include <climits>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

/* ************************************************************************** */
/*                                  Define                                    */
/* ************************************************************************** */

#ifdef WHITESPACE
#undef WHITESPACE
#define WHITESPACE " \t\n\r"
#else
#define WHITESPACE " \t\n\r"
#endif

/* ************************************************************************** */
/*                                   Enum                                     */
/* ************************************************************************** */

enum Result { OK, NO };
enum parseState { GLOBAL_BLOCK, SERVER_BLOCK, LOCATION_BLOCK };

/* ************************************************************************** */
/*                                 Exception                                  */
/* ************************************************************************** */

class ParsingException: public std::exception
{
	private:
		std::string	_msg;
	public:
		explicit ParsingException(const std::string &msg);
		explicit ParsingException(const char *msg);
		virtual ~ParsingException() throw();
		virtual const char	*what() const throw();
};

/* ************************************************************************** */
/*                              Location Config                               */
/* ************************************************************************** */

struct LocationData
{
	std::string					path;
	std::string					root;
	std::string					index;
	bool						autoindex;
	bool						upload_enabled;
	std::string					upload_path;
	bool						has_redirect;
	int							redirect_code;
	std::string					redirect_target;
	bool						cgi_enabled;
	std::string					cgi_extension;
	std::string					cgi_path;
	std::vector<std::string>	methods;

	LocationData()
	{
		this->autoindex = false;
		this->upload_enabled = false;
		this->has_redirect = false;
		this->redirect_code = 0;
		this->cgi_enabled = false;
	}
};

class LocationConfig
{
	private:
		LocationData	data;
	public:
		LocationConfig(const LocationData &cfg);
		~LocationConfig();
		const std::string&				getPath(void) const;
		const std::string&				getRoot(void) const;
		const std::string&				getIndex(void) const;
		const std::string&				getCgiExtension(void) const;
		const std::string&				getCgiPath(void) const;
		const std::string&				getUploadPath(void) const;
		const std::string&				getRedirectTarget(void) const;
		const std::vector<std::string>&	getMethods(void) const;
		int								getRedirectCode(void) const;
		bool							autoIndexEnabled(void) const;
		bool							uploadEnabled(void) const;
		bool							redirectEnabled(void) const;
		bool							cgiEnabled(void) const;
};

/* ************************************************************************** */
/*                               Server Config                                */
/* ************************************************************************** */

struct ListenConfig
{
	std::string	host;
	int			port;

	ListenConfig(const std::string &h, int p)
	{
		this->host = h;
		this->port = p;
	}
};

struct ServerData
{
	size_t						client_max_body_size;
	std::vector<ListenConfig>	listens;
	std::map<int, std::string>	error_pages;
	std::vector<LocationConfig>	locations;

	ServerData()
	{
		this->client_max_body_size = (1024*1024);
	}
};

class ServerConfig
{
	private:
		ServerData	data;
	public:
		ServerConfig(const ServerData &cfg);
		~ServerConfig();
		const std::vector<LocationConfig>	getLocations(void) const;
		size_t								getClientMaxBodySize(void) const;
		const std::map<int, std::string>	getErrorPages(void) const;
};

/* ************************************************************************** */
/*                                  Tokens                                    */
/* ************************************************************************** */

class Tokens
{
	private:
		std::vector<std::string>		_tokens;
		size_t							_index;
		std::string						_file;
		size_t							_lineNo;
		Result							_result;
		static std::string				trim(const std::string &s);
		static std::string				insertSpaces(const std::string &str, const std::string &trg);
		static std::string				stripComment(const std::string &line);
		static std::vector<std::string>	split(const std::string& line, const std::string& trg);
	public:
		Tokens(const std::string &filename);
		~Tokens();
		const std::string	&peek(void) const;
		void				tokenize(void);
		const std::string	&consume(void);
		void				expect(const std::string &got, const std::string &exp);
		void				reset(void);
		size_t				cursor(void) const;
		size_t				size(void) const;
};

/* ************************************************************************** */
/*                                Main Config                                 */
/* ************************************************************************** */

class Config
{
	private:
		typedef void (Config::*parseServerPTR)(void);
		typedef void (Config::*parseLocationPTR)(void);
		std::map<std::string, parseServerPTR>		serverFuncMap;
		std::map<std::string, parseLocationPTR>		locationFuncMap;
		Result						_result;
		parseState					_state;
		ServerData					_currentServer;
		LocationData				_currentLocation;
		std::vector<ServerConfig>	_servers;
		std::vector<parseState>		_stateStack;
		Tokens						_tokens;
	public:
		Config(const std::string &filepath);
		~Config();
		void	parse(void);
		const std::vector<ServerConfig>	&getServers() const;
	private:
		void	parseServerBlock(void);
		void	parseListen(void);
		void	parseClientMaxBodySize(void);
		void	parseLocation(void);
		void	parseErrorPage(void);
};

#endif