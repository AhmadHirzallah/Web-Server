#include <iostream>
#include "ConfigParser.hpp"

int	main(void)
{
	ConfigParser	parser("server.conf");

	try
	{
		parser.parse();
		// parser.print();
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return (1);
	}
}
