/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 20:40:00 by ahmad             #+#    #+#             */
/*   Updated: 2026/02/08 20:40:00 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

#include <sstream>
#include <cstdlib>
#include <cctype>
#include <cerrno>

/**
* @brief Construct a RequestParser with default size limits and state.
*
* @note - Default body size limit is 1 MiB.
*
*       - Default header size limit is 8 KiB.
*/
RequestParser::RequestParser()
	: _state(STATE_REQUEST_LINE),
	  _chunk_state(CHUNK_SIZE),
	  _max_body_size(1024 * 1024),
	  _max_header_size(8192),
	  _header_bytes(0),
	  _body_received(0),
	  _chunk_size(0),
	  _chunk_read(0),
	  _error_status(0)
{
}

/**
* @brief Set the maximum allowed body size in bytes.
*
* @param max    Maximum body size. If smaller than a request body, parsing fails.
*              This limit is enforced for both fixed-length and chunked bodies.
*
* @note - The limit is applied after headers are parsed.
*
*       - A violation results in status 413.
*/
void	RequestParser::setMaxBodySize(size_t max)
{
	_max_body_size = max;
}

/**
* @brief Set the maximum allowed header size in bytes.
*
* @param max    Maximum total size of request line and headers.
*              If the header section exceeds this limit, parsing fails.
*
* @note - This limit is enforced incrementally while reading headers.
*
*       - A violation results in status 400.
*/
void	RequestParser::setMaxHeaderSize(size_t max)
{
	_max_header_size = max;
}

/**
* @brief Access any unconsumed bytes still buffered by the parser.
*
* @note - Useful for debugging or pipelined request handling.
*
*       - Buffer content is owned by the parser.
*/
const std::string	&RequestParser::pendingBuffer() const
{
	return (_buffer);
}

/**
* @brief Get the HTTP status code associated with the last parse error.
*
* @note - Returns 0 if no error has occurred.
*
*       - The status is set when the parser enters error state.
*/
int	RequestParser::errorStatus() const
{
	return (_error_status);
}

/**
* @brief Get the human-readable message for the last parse error.
*
* @note - Returns an empty string if no error has occurred.
*
*       - The message is intended for internal diagnostics.
*/
const std::string	&RequestParser::errorMessage() const
{
	return (_error_message);
}

/**
* @brief Reset parser state for a new request.
*
* @param keep_buffer  When true, preserves unread bytes for pipelining.
*                     When false, clears the internal buffer entirely.
*
* @note - Resets all counters and parsing state to request-line mode.
*
*       - The internal Request object is cleared.
*/
void	RequestParser::reset(bool keep_buffer)
{
	_state = STATE_REQUEST_LINE;
	_chunk_state = CHUNK_SIZE;
	_header_bytes = 0;
	_body_received = 0;
	_chunk_size = 0;
	_chunk_read = 0;
	_error_status = 0;
	_error_message.clear();
	_req.clear();
	if (!keep_buffer)
		_buffer.clear();
}

/**
* @brief Transition the parser into error state and record details.
*
* @param status  HTTP status code to report (e.g., 400, 413).
* @param msg     Human-readable error message for diagnostics.
*
* @note - This also updates the embedded Request error fields.
*
*       - Subsequent calls to `feed` will return `PARSE_ERROR`.
*/
ParseResult	RequestParser::setError(int status, const std::string &msg)
{
	_state = STATE_ERROR;
	_error_status = status;
	_error_message = msg;
	_req.error_status = status;
	_req.error_message = msg;
	return (PARSE_ERROR);
}

/**
* @brief Pop a single LF-terminated line from the buffer.
*
* @param line   Output line without the line ending.
*              If the line ends with CRLF, the trailing CR is removed.
*              Only populated when the function returns true.
*
* @note - Returns false when no LF is currently available in the buffer.
*
*       - This allows tolerant parsing of either LF or CRLF input.
*/
bool	RequestParser::popLineLF(std::string &line)
{
	size_t pos = _buffer.find('\n');
	if (pos == std::string::npos)
		return (false);
	line.assign(_buffer, 0, pos);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	_buffer.erase(0, pos + 1);
	return (true);
}

/**
* @brief Pop a single CRLF-terminated line from the buffer.
*
* @param line   Output line without the CRLF terminator.
*              This function only succeeds when CRLF is present.
*              Only populated when the function returns true.
*
* @note - Returns false when CRLF is not yet available.
*
*       - Used specifically for strict chunked parsing.
*/
bool	RequestParser::popLineCRLF(std::string &line)
{
	size_t pos = _buffer.find("\r\n");
	if (pos == std::string::npos)
		return (false);
	line.assign(_buffer, 0, pos);
	_buffer.erase(0, pos + 2);
	return (true);
}

/**
* @brief Trim leading and trailing spaces or tabs from a string.
*
* @param s   Input string to trim.
*
* @note - Only spaces and tabs are removed.
*
*       - The original string is not modified.
*/
std::string	RequestParser::trim(const std::string &s)
{
	size_t start = 0;
	size_t end = s.size();
	while (start < end && (s[start] == ' ' || s[start] == '\t'))
		start++;
	while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t'))
		end--;
	return (s.substr(start, end - start));
}

/**
* @brief Convert a string to lowercase using ASCII rules.
*
* @param s   Input string to convert.
*
* @note - Characters are cast to unsigned char before `tolower`.
*
*       - This is sufficient for HTTP header normalization.
*/
std::string	RequestParser::toLower(const std::string &s)
{
	std::string out;
	out.reserve(s.size());
	for (size_t i = 0; i < s.size(); i++)
		out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(s[i]))));
	return (out);
}

/**
* @brief Parse a hexadecimal chunk size value.
*
* @param s    Input string containing hexadecimal digits.
* @param out  Parsed size in bytes when successful.
*             Only valid if the function returns true.
*
* @note - Does not accept empty strings.
*
*       - Rejects any non-hex character.
*/
bool	RequestParser::parseHexSize(const std::string &s, size_t &out)
{
	if (s.empty())
		return (false);
	size_t value = 0;
	for (size_t i = 0; i < s.size(); i++)
	{
		char c = s[i];
		int digit;
		if (c >= '0' && c <= '9')
			digit = c - '0';
		else if (c >= 'a' && c <= 'f')
			digit = 10 + (c - 'a');
		else if (c >= 'A' && c <= 'F')
			digit = 10 + (c - 'A');
		else
			return (false);
		value = value * 16 + digit;
	}
	out = value;
	return (true);
}

/**
* @brief Parse the HTTP request line into method, target, and version.
*
* @param line   Raw request line without line ending.
*               Must contain exactly three tokens.
*
* @note - Rejects lines with extra tokens.
*
*       - Requires version to start with "HTTP/".
*/
bool	RequestParser::parseRequestLine(const std::string &line)
{
	std::istringstream iss(line);
	std::string method;
	std::string target;
	std::string version;
	std::string extra;

	if (!(iss >> method >> target >> version))
		return (false);
	if (iss >> extra)
		return (false);
	if (method.empty() || target.empty() || version.empty())
		return (false);
	if (version.find("HTTP/") != 0)
		return (false);

	_req.method = method;
	_req.target = target;
	_req.version = version;

	size_t qpos = target.find('?');
	if (qpos == std::string::npos)
	{
		_req.path = target;
		_req.query.clear();
	}
	else
	{
		_req.path = target.substr(0, qpos);
		_req.query = target.substr(qpos + 1);
	}
	return (true);
}

/**
* @brief Parse and store a single header line.
*
* @param line   Header line without the line ending.
*               Must contain a colon separating name and value.
*
* @note - Rejects folded headers (leading whitespace).
*
*       - Disallows duplicate Content-Length and Transfer-Encoding.
*/
bool	RequestParser::parseHeaderLine(const std::string &line)
{
	if (line.empty())
		return (true);
	if (line[0] == ' ' || line[0] == '\t')
		return (false);

	size_t colon = line.find(':');
	if (colon == std::string::npos || colon == 0)
		return (false);

	std::string name = trim(line.substr(0, colon));
	std::string value = trim(line.substr(colon + 1));
	if (name.empty())
		return (false);

	name = toLower(name);
	if (_req.headers.count(name))
	{
		if (name == "content-length" || name == "transfer-encoding")
			return (false);
		_req.headers[name] += "," + value;
	}
	else
		_req.headers[name] = value;

	if (name == "host")
		_req.host = value;
	return (true);
}

/**
* @brief Get the raw value of a header by name.
*
* @param name   Header name (case-insensitive form expected).
*
* @note - Returns an empty string if the header is not present.
*
*       - Stored header keys are normalized to lowercase.
*/
std::string	RequestParser::headerValue(const std::string &name) const
{
	std::map<std::string, std::string>::const_iterator it = _req.headers.find(name);
	if (it == _req.headers.end())
		return ("");
	return (it->second);
}

/**
* @brief Check whether a header contains a specific comma-separated token.
*
* @param name   Header name to scan.
* @param token  Token to match after trimming and case-folding.
*
* @note - Header values are split on commas and trimmed.
*
*       - Matching is performed case-insensitively.
*/
bool	RequestParser::headerHasToken(const std::string &name, const std::string &token) const
{
	std::string value = headerValue(name);
	if (value.empty())
		return (false);
	std::string lower = toLower(value);
	std::string target = toLower(token);
	size_t start = 0;
	while (start < lower.size())
	{
		size_t end = lower.find(',', start);
		if (end == std::string::npos)
			end = lower.size();
		std::string part = trim(lower.substr(start, end - start));
		if (part == target)
			return (true);
		start = end + 1;
	}
	return (false);
}

/**
* @brief Parse Content-Length header into a size value.
*
* @param value  Raw header value containing a decimal number.
* @param out    Parsed size when successful.
*               Only valid if the function returns true.
*
* @note - Rejects non-digit characters.
*
*       - Rejects overflows reported by `strtoul`.
*/
bool	RequestParser::parseContentLength(const std::string &value, size_t &out)
{
	if (value.empty())
		return (false);
	for (size_t i = 0; i < value.size(); i++)
	{
		if (!std::isdigit(value[i]))
			return (false);
	}
	errno = 0;
	char *end = NULL;
	unsigned long v = std::strtoul(value.c_str(), &end, 10);
	if (end == value.c_str() || *end != '\0')
		return (false);
	if (errno == ERANGE)
		return (false);
	out = static_cast<size_t>(v);
	return (true);
}

/**
* @brief Finalize header-derived metadata and decide next parsing state.
*
* @note - Enforces mutual exclusion of chunked and Content-Length.
*
*       - Computes keep-alive rules for HTTP/1.0 and HTTP/1.1.
*/
void	RequestParser::finalizeHeaders()
{
	_req.is_chunked = headerHasToken("transfer-encoding", "chunked");
	bool has_cl = (_req.headers.count("content-length") != 0);

	if (_req.is_chunked && has_cl)
	{
		setError(400, "both transfer-encoding and content-length present");
		return ;
	}

	if (has_cl)
	{
		size_t cl = 0;
		if (!parseContentLength(headerValue("content-length"), cl))
		{
			setError(400, "invalid content-length");
			return ;
		}
		if (cl > _max_body_size)
		{
			setError(413, "request body too large");
			return ;
		}
		_req.content_length = cl;
	}

	if (_req.version == "HTTP/1.1")
		_req.keep_alive = !headerHasToken("connection", "close");
	else if (_req.version == "HTTP/1.0")
		_req.keep_alive = headerHasToken("connection", "keep-alive");
	else
		_req.keep_alive = false;

	if (_req.is_chunked)
		_state = STATE_BODY_CHUNKED;
	else if (_req.content_length > 0)
		_state = STATE_BODY_FIXED;
	else
		_state = STATE_DONE;
}

/**
* @brief Feed bytes into the parser and attempt to complete a request.
*
* @param bytes  Incoming bytes appended to the internal buffer.
*               Parsing only advances when complete lines or body segments exist.
* @param out    Output request populated on success.
*               Only valid when `PARSE_COMPLETE` is returned.
*
* @note - Returns `PARSE_NEED_MORE` if the request is incomplete.
*
*       - Returns `PARSE_ERROR` and sets error details on malformed input.
*/
ParseResult	RequestParser::feed(const std::string &bytes, Request &out)
{
	if (!bytes.empty())
		_buffer.append(bytes);

	if (_state == STATE_ERROR)
		return (PARSE_ERROR);
	if (_state == STATE_DONE)
		return (PARSE_COMPLETE);

	while (1)
	{
		if ((_state == STATE_REQUEST_LINE || _state == STATE_HEADERS)
			&& _buffer.size() > _max_header_size
			&& _buffer.find('\n') == std::string::npos)
			return (setError(400, "headers too large"));

		if (_state == STATE_REQUEST_LINE)
		{
			std::string line;
			if (!popLineLF(line))
				return (PARSE_NEED_MORE);
			if (line.empty())
				continue;
			if (!parseRequestLine(line))
				return (setError(400, "invalid request line"));
			_state = STATE_HEADERS;
			continue;
		}

		if (_state == STATE_HEADERS)
		{
			std::string line;
			if (!popLineLF(line))
				return (PARSE_NEED_MORE);
			_header_bytes += line.size() + 2;
			if (_header_bytes > _max_header_size)
				return (setError(400, "headers too large"));
			if (line.empty())
			{
				finalizeHeaders();
				if (_state == STATE_ERROR)
					return (PARSE_ERROR);
				if (_state == STATE_DONE)
				{
					out = _req;
					return (PARSE_COMPLETE);
				}
				continue;
			}
			if (!parseHeaderLine(line))
				return (setError(400, "invalid header"));
			continue;
		}

		if (_state == STATE_BODY_FIXED)
		{
			size_t need = _req.content_length - _body_received;
			if (_buffer.size() < need)
			{
				_req.body.append(_buffer);
				_body_received += _buffer.size();
				_buffer.clear();
				return (PARSE_NEED_MORE);
			}
			_req.body.append(_buffer, 0, need);
			_buffer.erase(0, need);
			_body_received += need;
			_state = STATE_DONE;
			out = _req;
			return (PARSE_COMPLETE);
		}

		if (_state == STATE_BODY_CHUNKED)
		{
			if (_chunk_state == CHUNK_SIZE)
			{
				std::string line;
				if (!popLineCRLF(line))
					return (PARSE_NEED_MORE);
				size_t semi = line.find(';');
				if (semi != std::string::npos)
					line = line.substr(0, semi);
				line = trim(line);
				if (!parseHexSize(line, _chunk_size))
					return (setError(400, "invalid chunk size"));
				_chunk_read = 0;
				if (_chunk_size == 0)
				{
					_chunk_state = CHUNK_TRAILERS;
					continue;
				}
				_chunk_state = CHUNK_DATA;
			}

			if (_chunk_state == CHUNK_DATA)
			{
				size_t need = _chunk_size - _chunk_read;
				if (_buffer.size() < need)
				{
					_req.body.append(_buffer);
					_chunk_read += _buffer.size();
					_body_received += _buffer.size();
					_buffer.clear();
					if (_body_received > _max_body_size)
						return (setError(413, "request body too large"));
					return (PARSE_NEED_MORE);
				}
				_req.body.append(_buffer, 0, need);
				_buffer.erase(0, need);
				_chunk_read += need;
				_body_received += need;
				if (_body_received > _max_body_size)
					return (setError(413, "request body too large"));
				_chunk_state = CHUNK_CRLF;
			}

			if (_chunk_state == CHUNK_CRLF)
			{
				if (_buffer.size() < 2)
					return (PARSE_NEED_MORE);
				if (_buffer[0] != '\r' || _buffer[1] != '\n')
					return (setError(400, "invalid chunk ending"));
				_buffer.erase(0, 2);
				_chunk_state = CHUNK_SIZE;
				continue;
			}

			if (_chunk_state == CHUNK_TRAILERS)
			{
				std::string line;
				if (!popLineCRLF(line))
					return (PARSE_NEED_MORE);
				if (line.empty())
				{
					_state = STATE_DONE;
					out = _req;
					return (PARSE_COMPLETE);
				}
				continue;
			}
		}
	}
}
