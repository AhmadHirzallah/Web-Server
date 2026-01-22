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

# include "ServerConfig.hpp"
# include <cstdlib>
# include <cerrno>
# include <climits>

#ifdef WHITESPACE
# undef WHITESPACE
# define WHITESPACE " \t\n\r"
#else
# define WHITESPACE " \t\n\r"
#endif

enum ParseResult
{
	OK,
	NOT_PARSED
};

enum ParseState
{
	GLOBAL_BLOCK,
	SERVER_BLOCK,
	LOCATION_BLOCK
};

class ConfigParser
{
	/*Private Attributes*/
	private:
		ParseResult	result;
		std::string	filename;
		std::vector<ParseState>	stateStack;
		std::vector<std::string>	tokens;
		std::vector<ServerConfig>	servers;
		ServerConfigData	currentServer;
		LocationConfigData	currentLocation;
		
		size_t	index;
	/*Public Methods*/
	public:
		ConfigParser(const std::string& file);
		void	print(void) const;
		void	parse(void);
		const std::vector<ServerConfig>&	getServers() const;
	/*Private Methods*/
	private:
		void	printState(void) const;
		void	printResult(void) const;
		void	printFilename(void) const;
		std::vector<std::string>	tokenizeFile(void);
		typedef void (ConfigParser::*parseServerPTR)(void);
		std::map<std::string, parseServerPTR>	serverFuncMap;
		typedef void (ConfigParser::*parseLocationPTR)(void);
		std::map<std::string, parseLocationPTR>	locationFuncMap;
		void	parseServerBlock(void);
		void	parseListen(void);
		void	parseErrorPage(void);
		void	parseClientMaxBodySize(void);
		void	parseLocation(void);
		const std::string&	peek(void) const;
		const std::string&	consume(void);
};

std::string	trim(const std::string& s);
std::vector<std::string>	split(const std::string& line, const std::string& delims);
std::string	insertSpacesAroundTargets(const std::string& s, const std::string& targets);
std::string	stripComment(const std::string& line);
void	printTokens(std::vector<std::string> tokens);
void	expect(const std::string& got, const std::string& expected);
bool	isValidIPv4(const std::string& ip);
int		parsePort(const std::string& s);
size_t	parseSize(const std::string& value);
int		parseHttpCode(const std::string& str);

#endif