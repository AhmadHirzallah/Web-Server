/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokens.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 16:28:11 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/29 16:28:11 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::string	Tokens::trim(const std::string &s)
{
	size_t	start;
	size_t	end;

	start = s.find_first_not_of(WHITESPACE);
	end = s.find_last_not_of(WHITESPACE);
	if (start == std::string::npos)
		return ("");
	return (s.substr(start, end - start + 1));
}

std::string	Tokens::insertSpaces(const std::string &str, const std::string &trg)
{
	std::string	result;

	for (std::size_t i = 0; i < str.size(); i++)
	{
		if (trg.find(str[i]) != std::string::npos)
		{
			result += ' ';
			result += str[i];
			result += ' ';
		}
		else
		{
			result += str[i];
		}
	}
	return (result);
}

std::string	Tokens::stripComment(const std::string &line)
{
	size_t	pos = line.find('#');
	
	if (pos != std::string::npos)
		return (line.substr(0, pos));
	return (line);
}

std::vector<std::string>	Tokens::split(const std::string& line, const std::string& trg)
{
	std::vector<std::string>	tokens;
	std::string					token;
	size_t						start;

	start = 0;
	while (true)
	{
		start = line.find_first_not_of(trg, start);
		if (start == std::string::npos)
			break ;
		size_t end = line.find_first_of(trg, start);
		if (end == std::string::npos)
			end = line.length();
		token = line.substr(start, end - start);
		tokens.push_back(token);
		start = end;
	}
	return (tokens);
}

Tokens::Tokens(const std::string &filepath)
{
	this->reset();
	this->_file = filepath;
	this->_result = NO;
}

const std::string	&Tokens::peek(void) const
{
	if (this->_index >= this->_tokens.size())
		throw (ParsingException("unexpected end of file `eof`"));
	return (this->_tokens[this->_index]);
}

const std::string	&Tokens::consume(void)
{
	const std::string	&value = peek();
	this->_index++;
	return (value);
}

void	Tokens::expect(const std::string &got, const std::string &exp)
{
	if (got != exp)
		throw (ParsingException("expected `" + exp + "`, got `" + got + "`"));
}

void	Tokens::reset(void)
{
	this->_index = 0;
}

size_t	Tokens::cursor(void) const
{
	return (this->_index);
}

size_t	Tokens::size(void) const
{
	return (this->_tokens.size());
}

void	Tokens::tokenize(void)
{
	std::ifstream				file(this->_file.c_str());
	std::vector<std::string>	result;
	std::string					line;

	if (this->_result == OK)
		return ;
	if (file.is_open() == false)
		throw (ParsingException("cannot open config file."));
	if (file.peek() == std::ifstream::traits_type::eof())
		throw (ParsingException("empty config file."));
	while (std::getline(file, line))
	{
		std::vector<std::string>	toks;
		this->_lineNo++;
		line = trim(line);
		if (line == "" || line[0] == '#')
			continue ;
		line = insertSpaces(line, "{};");
		line = stripComment(line);
		toks = split(line, WHITESPACE);
		result.insert(result.end(), toks.begin(), toks.end());
	}
	if (result.size() == 0)
		throw (ParsingException("empty config file."));
	this->_tokens = result;
	this->_result = OK; 
}

Tokens::~Tokens(){}
