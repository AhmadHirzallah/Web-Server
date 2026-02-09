/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:44:35 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/29 15:44:35 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

static int		parsePort(const std::string& s);
static bool		isValidIPv4(const std::string& ip);
static size_t	parseSize(const std::string& value);
static size_t	parseNumber(const std::string& str);
static int		parseHttpCode(const std::string& str);

ParsingException::ParsingException(const std::string &msg): _msg(msg){}
ParsingException::ParsingException(const char *msg): _msg(msg){}
ParsingException::~ParsingException() throw(){}
const char	*ParsingException::what() const throw()
{
	return (_msg.c_str());
}

Config::Config(const std::string &filepath): _result(NO), _tokens(Tokens(filepath))
{
	serverFuncMap["listen"] = &Config::parseListen;
	serverFuncMap["client_max_body_size"] = &Config::parseClientMaxBodySize;
	serverFuncMap["location"] = &Config::parseLocation;
	serverFuncMap["error_page"] = &Config::parseErrorPage;
	locationFuncMap["methods"] = &Config::parseMethods;
	// locationFuncMap["root"] = &Config::parseRoot;
	locationFuncMap["index"] = &Config::parseIndex;
	locationFuncMap["autoindex"] = &Config::parseAutoIndex;
	// locationFuncMap["upload_path"] = &Config::parseUploadPath;
	// locationFuncMap["return"] = &Config::parseReturn;
	// locationFuncMap["cgi"] = &Config::parseCGI;
}
Config::~Config(){}

const std::vector<ServerConfig>	&Config::getServers() const
{
	return (_servers);
}

void	Config::parse(void)
{
	if (_result == OK)
		return ;
	_tokens.tokenize();
	while (_tokens.cursor() < _tokens.size())
	{
		parseServerBlock();
	}
	_result = OK;
}

void	Config::parseServerBlock(void)
{
	_tokens.expect(_tokens.consume(), "server");
	_tokens.expect(_tokens.consume(), "{");
	_stateStack.push_back(SERVER_BLOCK);
	_currentServer = ServerData();
	while (_tokens.peek() != "}")
	{
		std::map<std::string, parseServerPTR>::const_iterator it = serverFuncMap.find(_tokens.peek());

		if (it != serverFuncMap.end())
		{
			parseServerPTR f = it->second;
			(this->*f)();
		}
		else
		{
			throw (ParsingException("unexpected token in `server` block `" + _tokens.peek() + "`"));
		}
	}
	_tokens.consume();
	_servers.push_back(ServerConfig(_currentServer));
	_stateStack.pop_back();
}

void	Config::parseLocation(void)
{
	_tokens.consume(); // `location`
	_tokens.expect(_tokens.consume(), "{");
	_stateStack.push_back(LOCATION_BLOCK);
	_currentLocation = LocationData();
	while (_tokens.peek() != "}")
	{
		std::map<std::string, parseLocationPTR>::const_iterator it = locationFuncMap.find(_tokens.peek());

		if (it != locationFuncMap.end())
		{
			parseLocationPTR f = it->second;
			(this->*f)();
		}
		else
		{
			throw (ParsingException("unexpected token in `location` block `" + _tokens.peek() + "`"));
		}
	}
	_tokens.consume();
	_currentServer.locations.push_back(_currentLocation);
	_stateStack.pop_back();
}

void	Config::parseListen(void)
{
	_tokens.consume();
	std::string	value = _tokens.consume();
	_tokens.expect(_tokens.peek(), ";");
	_tokens.consume();
	size_t		colon = value.find(':');
	std::string	host = "0.0.0.0";
	int			port;
	std::string	portStr = value.substr(colon + 1);

	if (colon == std::string::npos)
		port = parsePort(value);
	else
	{
		host = value.substr(0, colon);
		if (host.empty())
			throw (ParsingException("empty host in `listen` directive."));
		if (!isValidIPv4(host))
			throw (ParsingException("invalid IPv4 address `" + host + "` in `listen` directive."));
		port = parsePort(portStr);
	}
	for (std::size_t i = 0; i < _currentServer.listens.size(); i++)
	{
		if (_currentServer.listens[i].host == host && _currentServer.listens[i].port == port)
			throw (ParsingException("duplicate `listen` directive detected: `" + host + ":" + portStr + "`"));
	}
	_currentServer.listens.push_back(ListenConfig(host, port));
}

void	Config::parseErrorPage(void)
{
	std::vector<std::string>	tokens;

	_tokens.consume();
	while (_tokens.peek() != ";")
		tokens.push_back(_tokens.consume());
	_tokens.consume();
	if (tokens.size() < 2)
		throw (ParsingException("`error_page` directive requires at least one code and a path."));
	const std::string	&path = tokens.back();
	if (path[0] != '/')
		throw (ParsingException("`error_page` directive path must start with `/`"));
	for (std::size_t i = 0; i < tokens.size() - 1; i++)
	{
		const std::string	&codeStr = tokens[i];
		int					code = parseHttpCode(codeStr);
		_currentServer.error_pages[code] = path;
	}
}

void	Config::parseClientMaxBodySize(void)
{
	_tokens.consume();
	std::string	value = _tokens.consume();
	_tokens.expect(_tokens.peek(), ";");
	_tokens.consume();
	_currentServer.client_max_body_size = parseSize(value);
}

void	Config::parseAutoIndex(void)
{
	_tokens.consume();
	std::string	value = _tokens.consume();
	_tokens.expect(_tokens.peek(), ";");
	_tokens.consume();
	if (0 == value.compare("on"))
	{
		_currentLocation.autoindex = true;
	}
	else if (0 == value.compare("off"))
	{
		_currentLocation.autoindex = false;
	}
	else
	{
		throw (ParsingException("`autoindex` directive must only have an `on` or `off` value."));
	}
}

void	Config::parseMethods(void)
{
	_tokens.consume();
	std::vector<std::string>	methods;

	while (_tokens.peek() != ";")
	{
		std::string	m = _tokens.consume();

		if (m != "GET" && m != "POST" && m != "DELETE")
			throw (ParsingException("unsupported method `" + m + "` in `methods` directive."));
		for (std::size_t i = 0; i < methods.size(); i++)
		{
			if (methods[i] == m)
				throw (ParsingException("duplicate method `" + m + "` in `methods` directive."));
		}
		methods.push_back(m);
	}
	_tokens.expect(_tokens.consume(), ";");
	if (methods.empty())
		throw (ParsingException("`methods` directive cannot have no methods."));
	_currentLocation.methods = methods;
}

void	Config::parseIndex(void)
{
	_tokens.consume();
	if (_tokens.peek() == ";")
		throw (ParsingException("`index` directive must have a path."));
	std::string	value = _tokens.consume();
	_tokens.expect(_tokens.consume(), ";");
	_currentLocation.index = value;
}

static size_t	parseSize(const std::string& value)
{
	if (value.empty())
		throw (ParsingException("empty value in `client_max_body_size` directive."));
	char suffix = value[value.size() - 1];
	size_t 		multiplier = 1;
	std::string	numberPart = value;

	if (std::isalpha(suffix))
	{
		numberPart = value.substr(0, value.size() - 1);
		switch (std::toupper(suffix))
		{
			case 'K': multiplier = 1024; break;
			case 'M': multiplier = 1024 * 1024; break;
			case 'G': multiplier = 1024 * 1024 * 1024ULL; break;
			default:
				throw (ParsingException("invalid size suffix in `client_max_body_size` directive."));
		}
	}
	for (size_t i = 0; i < numberPart.size(); i++)
	{
		if (!std::isdigit(numberPart[i]))
			throw (ParsingException("invalid number `" + numberPart + "` in `client_max_body_size` directive."));
	}
	size_t number = parseNumber(numberPart);
	return (number * multiplier);
}

static size_t	parseNumber(const std::string& str)
{
	char	*end;
	errno = 0;
	const std::string	err_msg = "invalid number `" + str + "` in `client_max_body_size` directive.";

	unsigned long value = std::strtoul(str.c_str(), &end, 10);
	if (end == str.c_str())
		throw (ParsingException(err_msg));
	if (*end != '\0')
		throw (ParsingException(err_msg));
	if (errno == ERANGE || value == ULONG_MAX)
		throw (ParsingException(err_msg));
	return (static_cast<size_t>(value));
}

static int	parseHttpCode(const std::string& str)
{
	for (std::size_t i = 0; i < str.size(); i++)
	{
		if (!std::isdigit(str[i]))
			throw (ParsingException("invalid error code `" + str + "` in `error_page` directive."));
	}
	int code = std::atoi(str.c_str());
	if (code < 300 || code > 599)
		throw (ParsingException("`error_page` directive error code must be between 300 and 599."));
	return (code);
}

static bool	isValidIPv4(const std::string& ip)
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

static int	parsePort(const std::string& s)
{
	if (s.empty())
		throw (ParsingException("empty port in `listen` directive."));
	for (size_t i = 0; i < s.size(); i++)
	{
		if (!std::isdigit(s[i]))
			throw (ParsingException("invalid port `" + s + "` in `listen` directive."));
	}
	long port = std::strtol(s.c_str(), NULL, 10);
	if (port < 1 || port > 65535)
		throw (ParsingException("port out of range `"+ s +"` in `listen` directive."));
	return (static_cast<int>(port));
}
