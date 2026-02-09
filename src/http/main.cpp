/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 21:10:00 by ahmad             #+#    #+#             */
/*   Updated: 2026/02/08 21:10:00 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

#include <iostream>
#include <vector>

/**
* @brief Print a test result line to standard output.
*
* @param name   Label of the test case.
* @param ok     Indicates whether the test passed.
*              When false, the test is reported as failed.
*
* @note - Output goes to `std::cout`.
*
*       - Format is `[OK] name` or `[FAIL] name`.
*/
static void	printResult(const std::string &name, bool ok)
{
	std::cout << (ok ? "[OK]   " : "[FAIL] ") << name << std::endl;
}

/**
* @brief Execute a parsing scenario and validate its outcome.
*
* @param name           Label of the test case.
* @param chunks         Input pieces fed to the parser in order.
*                       Each chunk is appended before parsing continues.
* @param expect_ok      Expected success flag.
*                       When false, only error status is validated.
* @param expect_body    Expected body when parsing succeeds.
*                       Ignored when `expect_ok` is false.
* @param expect_status  Expected error status when parsing fails.
*                       Ignored when `expect_ok` is true.
*
* @note - Each test uses a fresh RequestParser instance.
*
*       - This function prints the result via `printResult`.
*/
static bool	runTest(const std::string &name,
					const std::vector<std::string> &chunks,
					bool expect_ok,
					const std::string &expect_body,
					int expect_status)
{
	RequestParser	parser;
	Request			req;
	ParseResult		r = PARSE_NEED_MORE;

	for (size_t i = 0; i < chunks.size(); i++)
		r = parser.feed(chunks[i], req);

	bool ok = true;
	if (expect_ok)
	{
		ok = (r == PARSE_COMPLETE);
		if (ok)
			ok = (req.body == expect_body);
	}
	else
	{
		ok = (r == PARSE_ERROR);
		if (ok)
			ok = (parser.errorStatus() == expect_status);
	}
	printResult(name, ok);
	return (ok);
}

/**
* @brief Program entry point for parser tests.
*
* @note - Returns 0 if all tests pass.
*
*       - Returns 1 if any test fails.
*/
int	main()
{
	size_t passed = 0;
	size_t total = 0;

	{
		std::vector<std::string> chunks;
		chunks.push_back("GET /index.html HTTP/1.1\r\n");
		chunks.push_back("Host: example.com\r\n\r\n");
		total++;
		if (runTest("simple GET", chunks, true, "", 0))
			passed++;
	}
	{
		std::vector<std::string> chunks;
		chunks.push_back("POST /upload HTTP/1.1\r\nHost: a\r\n");
		chunks.push_back("Content-Length: 11\r\n\r\n");
		chunks.push_back("hello world");
		total++;
		if (runTest("POST with Content-Length", chunks, true, "hello world", 0))
			passed++;
	}
	{
		std::vector<std::string> chunks;
		chunks.push_back("POST /cgi HTTP/1.1\r\n");
		chunks.push_back("Transfer-Encoding: chunked\r\n\r\n");
		chunks.push_back("5\r\nhello\r\n6\r\n world\r\n0\r\n\r\n");
		total++;
		if (runTest("POST chunked body", chunks, true, "hello world", 0))
			passed++;
	}
	{
		std::vector<std::string> chunks;
		chunks.push_back("BADREQUEST\r\n\r\n");
		total++;
		if (runTest("bad request line", chunks, false, "", 400))
			passed++;
	}
	{
		RequestParser parser;
		parser.setMaxBodySize(4);
		Request req;
		ParseResult r = parser.feed("POST / HTTP/1.1\r\nContent-Length: 5\r\n\r\nhello", req);
		bool ok = (r == PARSE_ERROR && parser.errorStatus() == 413);
		printResult("body size limit (413)", ok);
		total++;
		if (ok)
			passed++;
	}

	std::cout << "Passed " << passed << " / " << total << " tests" << std::endl;
	return (passed == total ? 0 : 1);
}
