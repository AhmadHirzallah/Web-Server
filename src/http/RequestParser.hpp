/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 20:40:00 by ahmad             #+#    #+#             */
/*   Updated: 2026/02/08 20:40:00 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "Request.hpp"

class RequestParser
{
	public:
		RequestParser();
		void		setMaxBodySize(size_t max);
		void		setMaxHeaderSize(size_t max);
		ParseResult	feed(const std::string &bytes, Request &out);
		void		reset(bool keep_buffer);
		int			errorStatus() const;
		const std::string	&errorMessage() const;
		const std::string	&pendingBuffer() const;

	private:
		enum State
		{
			STATE_REQUEST_LINE,
			STATE_HEADERS,
			STATE_BODY_FIXED,
			STATE_BODY_CHUNKED,
			STATE_DONE,
			STATE_ERROR
		};

		enum ChunkState
		{
			CHUNK_SIZE,
			CHUNK_DATA,
			CHUNK_CRLF,
			CHUNK_TRAILERS
		};

		std::string	_buffer;
		State		_state;
		ChunkState	_chunk_state;
		size_t		_max_body_size;
		size_t		_max_header_size;
		size_t		_header_bytes;
		size_t		_body_received;
		size_t		_chunk_size;
		size_t		_chunk_read;
		Request		_req;
		int			_error_status;
		std::string	_error_message;

		ParseResult	setError(int status, const std::string &msg);
		bool		popLineLF(std::string &line);
		bool		popLineCRLF(std::string &line);
		bool		parseRequestLine(const std::string &line);
		bool		parseHeaderLine(const std::string &line);
		void		finalizeHeaders();
		bool		headerHasToken(const std::string &name, const std::string &token) const;
		std::string	headerValue(const std::string &name) const;
		bool		parseContentLength(const std::string &value, size_t &out);

		static std::string	trim(const std::string &s);
		static std::string	toLower(const std::string &s);
		static bool			parseHexSize(const std::string &s, size_t &out);
};

#endif
