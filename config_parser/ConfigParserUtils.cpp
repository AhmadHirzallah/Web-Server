/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParserUtils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 18:15:42 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/19 18:15:42 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

std::string	trim(const std::string& s)
{
	size_t	start = s.find_first_not_of(" \t\r\n");
	size_t	end = s.find_last_not_of(" \t\r\n");
	if (start == std::string::npos)
		return ("");
	return (s.substr(start, end - start + 1));
}

bool	startsWith(const std::string& s, const std::string& prefix)
{
	return (s.compare(0, prefix.length(), prefix) == 0);
}

std::vector<std::string>	split(const std::string& line, const std::string& delims)
{
	std::vector<std::string>	tokens;
	std::string		token;
	size_t			start;

	start = 0;
	while (true)
	{
		start = line.find_first_not_of(delims, start);
		if (start == std::string::npos)
			break;
		size_t end = line.find_first_of(delims, start);
		if (end == std::string::npos)
			end = line.length();
		token = line.substr(start, end - start);
		tokens.push_back(token);
		start = end;
	}
	return (tokens);
}

void	printTokens(std::vector<std::string> tokens)
{
	#ifdef PARSE_DEBUG
	std::cout << "\033[35mOutput: \033[0m";
	for (size_t i = 0; i < tokens.size(); i++)
	{
		std::cout << "'" << tokens[i] << "' ";
	}
	std::cout << std::endl;
	#endif
}

std::string	insertSpacesAroundTargets(const std::string& s, const std::string& targets)
{
	std::string	result;

	for (size_t i = 0; i < s.size(); i++)
	{
		if (targets.find(s[i]) != std::string::npos)
		{
			result += ' ';
			result += s[i];
			result += ' ';
		}
		else
		{
			result += s[i];
		}
	}
	return (result);
}

std::string	stripComment(const std::string& line)
{
	size_t	pos = line.find('#');
	if (pos != std::string::npos)
	{
		#ifdef PARSE_DEBUG
		std::cout << "\033[36mComment stripped:\033[0m '" << line.substr(pos + 1) << "' " << std::endl;
		#endif
		return (line.substr(0, pos));
	}
	return (line);
}

void	expect(const std::string& got, const std::string& expected)
{
	if (got != expected)
		throw (std::runtime_error("Expected '" + expected + "', got '" + got + "'"));
}