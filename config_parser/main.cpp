#include <iostream>
#include "ConfigParser.hpp"

int	main(void)
{
	/*LocationConfigData cfg;

	cfg.autoindex = (true);
	cfg.cgi_enabled = (true);
	cfg.redirect_code = (int)404;
	cfg.path = "/home/path";
	cfg.index = "index.html";
	cfg.cgi_extension = ".php";
	cfg.root = "/root";
	cfg.upload_path = "/upload";
	cfg.upload_enabled = (true);
	cfg.redirect_target = "/target";
	cfg.has_redirect = (true);
	cfg.cgi_path = "/bin/cgi";
	cfg.methods.push_back("GET");
	cfg.methods.push_back("POST");

	LocationConfig	loc(cfg);
	LocationConfig	test;

	ServerConfigData servdata;

	std::map<int, std::string> error;
	error[404] = "/errors/404.html";
	error[505] = "/errors/505.html";
	servdata.client_max_body_size = 12365436;
	servdata.hosts.push_back("127.0.0.1");
	servdata.hosts.push_back("192.168.1.2");
	servdata.locations.push_back(loc);
	servdata.ports.push_back(8080);
	servdata.ports.push_back(8081);
	servdata.error_pages = error;

	ServerConfig serv(servdata);

	serv.print();*/

	ConfigParser	parser("server.conf");

	try
	{
		parser.parse();
	}
	catch(const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return (1);
	}
}
