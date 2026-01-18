/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 23:05:10 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/10 23:05:11 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

std::string	trim(const std::string& s);

ConfigParser::ConfigParser(const std::string& file): result(NOT_PARSED),
													state(GLOBAL_BLOCK),
													filename(file){}

ConfigParser::~ConfigParser(){}

std::vector<std::string>	ConfigParser::tokenizeFile(void)
{
	std::ifstream	file(this->filename.c_str());
	std::vector<std::string>	result;
	size_t		lineNumber = 0;
	std::string	line;

	if (file.is_open() == false)
		throw (std::runtime_error("Cannot open config file."));
	if (file.peek() == std::ifstream::traits_type::eof())
		throw (std::runtime_error("Empty config file."));
	while (std::getline(file, line))
	{
		std::vector<std::string>	tokens;
		lineNumber++;
		line = trim(line);
		if (line == "" || line[0] == '#')
		{
			#ifdef PARSE_DEBUG
				std::cout << "\033[33mSkipping line [" << lineNumber << "]:\033[0m '" << line << "' " << std::endl;
			#endif
			continue ;
		}
		line = insertSpacesAroundTargets(line, "{};");
		line = stripComment(line);
		tokens = split(line, WHITESPACE);
		#ifdef PARSE_DEBUG
			std::cout << "\033[32mSplitted line [" << lineNumber << "]: \033[0m";
			printTokens(tokens);
		#endif
		result.insert(result.end(), tokens.begin(), tokens.end());
	}
	if (result.size() == 0)
		throw (std::runtime_error("Empty config file."));
	return (result);
}

void	ConfigParser::parse(void)
{
	if (this->result == OK)
		return ;
	this->printFilename();
	this->tokens = this->tokenizeFile();
	printTokens(this->tokens);
	this->printState();
	/*Parse Tokens here*/
	this->printResult();
}

void	ConfigParser::print() const
{
	for (size_t i = 0; i < this->servers.size(); i++)
		this->servers[i].print();
}

const std::vector<ServerConfig>&	ConfigParser::getServers() const
{
	return (this->servers);
}

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

void	ConfigParser::printState(void) const
{
	#ifdef PARSE_DEBUG
	std::cout << "Parsing State: ";
	if (state == GLOBAL_BLOCK)
		std::cout << "\033[1mGLOBAL\033[0m" << std::endl;
	else if (state == SERVER_BLOCK)
		std::cout << "\033[1mSERVER\033[0m" << std::endl;
	else
		std::cout << "\033[1mLOCATION\033[0m" << std::endl;
	#endif
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

void	ConfigParser::printResult(void) const
{
	#ifdef PARSE_DEBUG
	std::cout << "Parsing Result: ";
	if (result == OK)
		std::cout << "\033[32mOK\033[0m" << std::endl;
	else
		std::cout << "\033[31mNOT_PARSED\033[0m" << std::endl;
	#endif
}

void	ConfigParser::printFilename(void) const
{
	#ifdef PARSE_DEBUG
	std::cout << "\033[34mFilename: \033[0m" << this->filename << std::endl;
	#endif
}