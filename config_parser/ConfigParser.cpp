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

ConfigParser::ConfigParser(const std::string& file): result(NOT_PARSED),
													filename(file),
													stateStack(1, GLOBAL_BLOCK),
													index(0)
{
	serverFuncMap["listen"] = &ConfigParser::parseListen;
	serverFuncMap["client_max_body_size"] = &ConfigParser::parseClientMaxBodySize;
	serverFuncMap["location"] = &ConfigParser::parseLocation;
	serverFuncMap["error_page"] = &ConfigParser::parseErrorPage;
	// locationFuncMap["methods"] = &ConfigParser::parseMethods;
	// locationFuncMap["root"] = &ConfigParser::parseRoot;
	// locationFuncMap["index"] = &ConfigParser::parseIndex;
	// locationFuncMap["autoindex"] = &ConfigParser::parseAutoIndex;
	// locationFuncMap["upload_path"] = &ConfigParser::parseUploadPath;
	// locationFuncMap["return"] = &ConfigParser::parseReturn;
	// locationFuncMap["cgi"] = &ConfigParser::parseCGI;
}

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
	{
		this->printResult();
		return ;
	}
	this->printFilename();
	this->printResult();
	this->tokens = this->tokenizeFile();
	printTokens(this->tokens);
	this->printState();
	while (index < this->tokens.size())
	{
		parseServerBlock();
	}
	result = OK;
	this->printState();
	this->printResult();
}

void	ConfigParser::parseServerBlock(void)
{
	expect(consume(), "server");
	expect(consume(), "{");
	stateStack.push_back(SERVER_BLOCK);
	this->printState();
	currentServer = ServerConfigData();
	while (peek() != "}")
	{
		std::map<std::string, parseServerPTR>::iterator	it = serverFuncMap.find(peek());
		if (it != serverFuncMap.end())
		{
			parseServerPTR f = it->second;
			(this->*f)();
		}
		else
			throw (std::runtime_error("Unexpected token in server block '" + peek() + "'"));
	}
	consume();
	servers.push_back(ServerConfig(currentServer));
	stateStack.pop_back();
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

void	ConfigParser::printState(void) const
{
	#ifdef PARSE_DEBUG
	std::cout << "Parsing State: ";
	if (stateStack.back() == GLOBAL_BLOCK)
		std::cout << "\033[1mGLOBAL\033[0m" << std::endl;
	else if (stateStack.back() == SERVER_BLOCK)
		std::cout << "\033[1mSERVER\033[0m" << std::endl;
	else
		std::cout << "\033[1mLOCATION\033[0m" << std::endl;
	#endif
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

const std::string&	ConfigParser::peek(void) const
{
	if (index >= tokens.size())
		throw std::runtime_error("Unexpected end of file");
	return tokens[index];
}

const std::string&	ConfigParser::consume(void)
{
	const std::string& tok = peek();
	index++;
	return tok;
}

void	ConfigParser::parseListen(void)
{
	consume(); // will consume as instructed later
	std::cout << "parseListen" << std::endl;
}

void	ConfigParser::parseErrorPage(void)
{
	consume();
	std::cout << "parseErrorPage" << std::endl;
}

void	ConfigParser::parseClientMaxBodySize(void)
{
	consume();
	std::cout << "parseClientMaxBodySize" << std::endl;
}

void	ConfigParser::parseLocation(void)
{
	consume();
	std::cout << "parseLocation" << std::endl;
}