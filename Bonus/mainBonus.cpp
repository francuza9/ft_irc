#include "Bot.hpp"

bool	parseEnv(std::vector<std::string> &);
bool	validPort(std::string);
void	SignalHandler(int);
void	sendMessage(std::string message, int fd);
int		sock;

int	main(int argc, char **argv)
{
	Bot bot;
	std::vector<std::string> vals;

	if (parseEnv(vals))
		return 1;
	
	if (argc > 1)
	{
		vals[PORT] = argv[1];
		if (argc == 3)
			vals[PASSWORD] = argv[2];
		if (argc > 3)
		{
			std::cerr << "BOT: Too many arguments." << std::endl;
			return (1);
		}
	}

	if (!validPort(vals[PORT]) || vals[PASSWORD].empty() || vals[PASSWORD].size() > 20)
	{
		std::cerr << "Invalid port! / password!" << std::endl;
		return (1);
	}

	signal(SIGINT, SignalHandler);
	signal(SIGQUIT, SignalHandler);

	std::string address = vals[ADDRESS];
	if(address == "localhost" || address == "LOCALHOST")
		address = "127.0.0.1";
	
	struct sockaddr_in ircHints;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		std::cerr << "failed to create socket" << std::endl;
		return (1);
	}
    ircHints.sin_family = AF_INET;
    ircHints.sin_port = htons(std::atoi(vals[PORT].c_str()));
	ircHints.sin_addr.s_addr = inet_addr(address.c_str());
	
    if(connect(sock, (struct sockaddr*)&ircHints, sizeof(ircHints)) == -1)
	{
		close(sock);
		std::cerr << "connect() failed." << std::endl;
		return (1);
	}
	
    // connection to irc server
    sendMessage("PASS " + std::string(vals[PASSWORD]) + "\r\n", sock);
    sendMessage("NICK " + std::string(vals[NICKNAME]) + "\r\n", sock);
    sendMessage("USER bot 0 * bot\r\n", sock);

	bot.setNick(std::string(vals[NICKNAME]));
	std::cout << "BOT Is Connected!" << std::endl;
	
	bot.init(sock);
	std::cout << "BOT Is Disconnected!" << std::endl;
	return (0);
}

bool	parseEnv(std::vector<std::string> &vals)
{
	std::vector<std::pair<std::string, std::string> > envs;
	std::string content = "",line, key, value;
	std::fstream file("Bonus/.env");

	if (!file.is_open())
	{
		std::cerr << "Failed to open .env file" << std::endl;
		return (true);
	}
	
	while (std::getline(file, line))
		content += line + '\n';
	
	file.close();
	
	std::stringstream ss(content);
	while (std::getline(ss, line, '\n'))
	{
		std::stringstream ss2(line);
		std::getline(ss2, key, '=');
		std::getline(ss2, value, '=');
		envs.push_back(std::make_pair(key, value));
	}

	if (envs.size() != 4)
	{
		std::cerr << "Invalid number of ENV variables" << std::endl;
		return (true);
	}
	
	vals.push_back("ADDRESS");
	vals.push_back("PORT");
	vals.push_back("PASSWORD");
	vals.push_back("NICKNAME");
	
	for (size_t i = 0; i < envs.size(); i++)
	{
		if (envs[i].first != vals[i])
		{
			std::cerr << "Invalid ENV variable" << std::endl;
			return (true);
		}
		if ( i != 2 && (std::isspace(envs[i].second[0]) || std::isspace(envs[i].second[envs[i].second.size() - 1])))
		{
			std::cerr << "Invalid ENV values" << std::endl;
			return (true);
		}
	}
	for (size_t i = 0; i < envs.size(); i++)
		vals[i] = envs[i].second;
	return (false);
}
bool validPort(std::string port)
{
	size_t	i;

	i = port.find_first_not_of(" ");
	if (i == std::string::npos)
		return (false);
	port.erase(0,i);
	i = port.find_first_not_of("0");
	if (i == std::string::npos)
		return (false);
	port.erase(0, i);
	if (port.find_first_not_of("0123456789") != std::string::npos)
		return (false);
	if (port.size() > 10 || (port.size() == 10 && port > "2147483647"))
		return (false);
	if (std::atoi(port.c_str()) < 1024 || std::atoi(port.c_str()) > 65535)
		return (false);
	return (true);
}

void	sendMessage(std::string message, int fd)
{
    if(send(fd, message.c_str(), message.size(), 0) == -1)
        std::cerr << "Send failed" << std::endl;
}

void	SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received !" << std::endl;
	sendMessage("QUIT\r\n", sock);
	close(sock);
}
