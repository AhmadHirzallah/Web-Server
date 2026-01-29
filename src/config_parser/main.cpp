/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 20:13:56 by drahwanj          #+#    #+#             */
/*   Updated: 2026/01/29 20:13:56 by drahwanj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Config.hpp"

int	main(void)
{
	Config	parser("server.conf");

	try
	{
		parser.parse();
	}
	catch (const std::exception &e)
	{
		std::cerr << "webserv: " << e.what() << std::endl;
		return (1);
	}
}
