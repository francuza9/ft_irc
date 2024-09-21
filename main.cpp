#include "Server.hpp"

int checkPort(std::string portStr)
{
	if (portStr.empty())
		return (-1);
	if (portStr.find_first_not_of("0123456789") != std::string::npos)
		return (-1);
	if (portStr[0] == '0')
		portStr = portStr.substr(portStr.find_first_not_of('0'));
	if (portStr.length() > 5 || portStr.length() < 4)
		return (-1);
	int port = atoi(portStr.c_str());
	if (port < 1024 || port > 65535)
		return (-1);
	return (port);
}

int	main(int argc, char **argv)
{
	argc--;
	argv++;
	
	if (argc != 2 || checkPort(argv[0]) == -1)
	{
		if (argc != 2)
			std::cerr << "2 Arguments required... (passed " << argc << ")." << std::endl;
		else
			std::cerr << "Port " << argv[0] << " is invalid." << std::endl;
		return (1);
	}
	Server ser;
	std::cout << "---- SERVER ----" << std::endl;
	try
	{
		signal(SIGINT, Server::sigHandler);
		signal(SIGQUIT, Server::sigHandler);
		ser.serverInit(checkPort(argv[0]), argv[1]);
	}
	catch (std::exception &x)
	{
		ser.closeFds();
		std::cerr << x.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}