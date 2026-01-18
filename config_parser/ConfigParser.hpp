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
		ParseState	state;
		std::string	filename;
		std::vector<std::string>	tokens;
		std::vector<ServerConfig>	servers;
	/*Public Methods*/
	public:
		ConfigParser(const std::string& file);
		~ConfigParser();
		void	print(void) const;
		void	parse(void);
		const std::vector<ServerConfig>&	getServers() const;
	/*Private Methods*/
	private:
		void	printState(void) const;
		void	printResult(void) const;
		void	printFilename(void) const;
		std::vector<std::string>	tokenizeFile(void);
};

std::string	trim(const std::string& s);
bool	startsWith(const std::string& s, const std::string& prefix);
std::vector<std::string>	split(const std::string& line, const std::string& delims);
std::string	insertSpacesAroundTargets(const std::string& s, const std::string& targets);
std::string	stripComment(const std::string& line);
void	printTokens(std::vector<std::string> tokens);

#endif