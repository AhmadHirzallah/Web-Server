/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 20:40:00 by ahmad             #+#    #+#             */
/*   Updated: 2026/02/08 20:40:00 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <cstddef>

struct Request
{
	std::string	method;
	std::string	target;
	std::string	path;
	std::string	query;
	std::string	version;
	std::string	host;
	std::map<std::string, std::string>	headers;
	std::string	body;
	size_t		content_length;
	bool		is_chunked;
	bool		keep_alive;
	int			error_status;
	std::string	error_message;

	Request();
	void	clear();
};

enum ParseResult
{
	PARSE_NEED_MORE,
	PARSE_COMPLETE,
	PARSE_ERROR
};

#endif
