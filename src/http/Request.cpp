/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 20:40:00 by ahmad             #+#    #+#             */
/*   Updated: 2026/02/08 20:40:00 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/**
* @brief Construct a Request and initialize all fields to their defaults.
*
* @note - This constructor immediately delegates to `clear()` for consistency.
*
*       - All scalar flags are reset to safe defaults.
*/
Request::Request()
{
	clear();
}

/**
* @brief Reset all request fields to their default/empty values.
*
* @note - Clears parsing artifacts such as headers and body.
*
*       - Does not affect any external parser state.
*/
void	Request::clear()
{
	method.clear();
	target.clear();
	path.clear();
	query.clear();
	version.clear();
	host.clear();
	headers.clear();
	body.clear();
	content_length = 0;
	is_chunked = false;
	keep_alive = false;
	error_status = 0;
	error_message.clear();
}
