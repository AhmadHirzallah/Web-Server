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
	#else
		(void)tokens;
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
		throw (std::runtime_error("webserv: expected '" + expected + "', got '" + got + "'"));
}

int	parsePort(const std::string& s)
{
	if (s.empty())
		throw (std::runtime_error("webserv: empty port in \"listen\" directive"));

	for (size_t i = 0; i < s.size(); i++)
	{
		if (!std::isdigit(s[i]))
			throw (std::runtime_error("webserv: invalid port \"" + s + "\" in \"listen\" directive"));
	}
	long port = std::strtol(s.c_str(), NULL, 10);
	if (port < 1 || port > 65535)
		throw (std::runtime_error("webserv: port out of range \""+ s +"\" in \"listen\" directive "));

	return (static_cast<int>(port));
}

int	parseHttpCode(const std::string& str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!std::isdigit(str[i]))
			throw (std::runtime_error("webserv: invalid error code \"" + str + "\" in \"error_page\" directive"));
	}
	int code = std::atoi(str.c_str());
	if (code < 300 || code > 599)
		throw (std::runtime_error("webserv: \"error_page\" directive error code must be between 300 and 599"));
	return (code);
}

bool	isValidIPv4(const std::string& ip)
{
	int	dots = 0;
	size_t	start = 0;

	for (size_t i = 0; i <= ip.size(); i++)
	{
		if (i == ip.size() || ip[i] == '.')
		{
			if (i == start)
				return (false);

			std::string part = ip.substr(start, i - start);

			for (size_t j = 0; j < part.size(); j++)
				if (!std::isdigit(part[j]))
					return (false);
			int n = std::atoi(part.c_str());
			if (n < 0 || n > 255)
				return (false);
			start = i + 1;
			dots++;
		}
	}
	return (dots == 4);
}

static size_t	parseNumber(const std::string& str)
{
	char	*end;
	errno = 0;
	const std::string	err_msg = "webserv: invalid number \"" + str + "\" in \"client_max_body_size\" directive";

	unsigned long value = std::strtoul(str.c_str(), &end, 10);
	if (end == str.c_str())
		throw (std::runtime_error(err_msg));
	if (*end != '\0')
		throw (std::runtime_error(err_msg));
	if (errno == ERANGE || value == ULONG_MAX)
		throw (std::runtime_error(err_msg));
	return (static_cast<size_t>(value));
}

size_t	parseSize(const std::string& value)
{
	if (value.empty())
		throw (std::runtime_error("webserv: empty value in \"client_max_body_size\" directive"));
	char suffix = value[value.size() - 1];
	size_t multiplier = 1;
	std::string numberPart = value;

	if (std::isalpha(suffix))
	{
		numberPart = value.substr(0, value.size() - 1);
		switch (std::toupper(suffix))
		{
			case 'K': multiplier = 1024; break;
			case 'M': multiplier = 1024 * 1024; break;
			case 'G': multiplier = 1024 * 1024 * 1024ULL; break;
			default:
				throw (std::runtime_error("webserv: invalid size suffix in \"client_max_body_size\" directive"));
		}
	}
	for (size_t i = 0; i < numberPart.size(); i++)
	{
		if (!std::isdigit(numberPart[i]))
			throw (std::runtime_error("webserv: invalid number \"" + numberPart + "\" in \"client_max_body_size\" directive"));
	}
	size_t number = parseNumber(numberPart);
	return (number * multiplier);
}
