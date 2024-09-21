#include "Server.hpp"

bool Server::_endSignal = false;

void	Server::sigHandler(int signum)
{
	(void)signum;
	Server::_endSignal = true;
	std::cout << std::endl << "Signal Recieved !" << std::endl;
}

void	Server::serverInit(int port, std::string pass)
{
	_port = port;
	_password = pass;
	serverSocket();
	std::cout << "Server <" << _serSocketFd << "> Connected" << std::endl;
	std::cout << "Waiting to accept a connection...\n";

	while (!Server::_endSignal)
	{
		if ((poll(&_fds[0], (nfds_t)_fds.size(), -1) ==  -1)&& !Server::_endSignal) // CHANGE -1 TO 0 TO TEST
			throw(std::runtime_error("poll failed."));
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN) // checks if theres any data to read
			{
				if (_fds[i].fd == _serSocketFd)
					acceptClient();
				else
					recieveData(_fds[i].fd);
			}
		}
	}
	closeFds();
}

void	Server::serverSocket()
{
	struct sockaddr_in	address;
	struct pollfd		newPoll;
	int					n = 1;

	address.sin_family = AF_INET; // for ipv4
	address.sin_port = htons(_port); // htons to convert from port to network bytes
	address.sin_addr.s_addr = INADDR_ANY; // setting address to any available local host machine address

	_serSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serSocketFd == -1)
		throw (std::runtime_error("Socket Failed."));
	
	if (setsockopt(_serSocketFd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n)) == -1)
		throw (std::runtime_error("Setting socket to REUSEADDR failed."));
	if (fcntl(_serSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("Setting socket to NONBLOCK failed.")); // for no block
	if (bind(_serSocketFd, (struct sockaddr *)&address, sizeof(address)) == -1)
		throw (std::runtime_error("Failed to bind the socket."));
	if (listen(_serSocketFd, SOMAXCONN/*128 i think*/) == -1)
		throw (std::runtime_error("Listen failed."));
	newPoll.fd = _serSocketFd;
	newPoll.events = POLLIN; // For reading data
	newPoll.revents = 0;
	_fds.push_back(newPoll);
}

void	Server::acceptClient()
{
	struct sockaddr_in	clientAddr;
	struct pollfd		newPollFd;
	Client				client;
	socklen_t			len = sizeof(clientAddr);
	int					incomingFd = accept(_serSocketFd, (sockaddr *)&(clientAddr), &len);

	if (incomingFd == -1)
		throw (std::runtime_error("accept failed."));
	if (fcntl(incomingFd, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("failed to set NONBLOCK option."));

	newPollFd.fd = incomingFd;
	newPollFd.revents = 0;
	newPollFd.events = POLLIN; // For reading data

	client.setFd(incomingFd);
	client.setIPAddr(inet_ntoa(clientAddr.sin_addr));  // inet_ntoa for converting ip address to string
	_clients.push_back(client);
	_fds.push_back(newPollFd);

	if (_password.empty())
		client.setRegistered(true);

	std::cout << "Client <" << incomingFd << "> Connected" << std::endl;
}

void	Server::recieveData(int fd)
{
	std::vector<std::string>	command;
	Client						*client = getClient(fd);
	char						buffer[1024];
	

	memset(buffer, 0, sizeof(buffer));
	ssize_t	bytes = recv(fd, buffer, sizeof(buffer) - 1, 0); // this is to recieve data
	if (bytes <= 0)
	{
		std::string					tempNick = client->getID();
		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i].is_member(*client))
			{
				if (_channels[i].remove_member(*client) == 1)
					_channels.erase(_channels.begin() + i);
			}
			if (_channels[i].is_invited(*client))
				_channels[i].remove_invite(*client);
		}
		clearClient(fd);
		sendToClient(RPL_CLIENTQUIT(tempNick, std::string("")));
	}
	else  // Parsing starts here
	{
		client->setBuffer(buffer);
		if (client->getBuffer().find_first_of("\r\n") == std::string::npos)
			return;
		command = splitBuffer(client->getBuffer());
		for (size_t i = 0; i < command.size(); i++)
			selectCommand(fd, command[i]);
		if (getClient(fd))
			getClient(fd)->clearBuffer();
	}
}

std::vector<std::string>	Server::splitBuffer(std::string str)
{
	std::vector<std::string>	temp;
	std::istringstream			stream(str);
	std::string					line;
	size_t						i;
	while(getline(stream, line))
	{
		i = line.find_first_of("\r\n");
		if (i != std::string::npos)
			line = line.substr(0, i);
		temp.push_back(line);
	}
	return (temp);
}

std::vector<std::string>	Server::splitCommand(std::string str)
{
	std::vector<std::string>	temp;
	std::istringstream			stream(str);
	std::string					token;
	while(stream >> token)
	{
		temp.push_back(token);
		token.clear();
	}
	return (temp);
}

std::vector<std::string>	Server::getSplitParams(std::string str, unsigned int paramLen)
{
	std::vector<std::string>	temp;
	size_t						i;
	
	str.erase(0, paramLen);
	while (!str.empty())
	{
		i = str.find_first_not_of(" ");
		if (i == std::string::npos)
			return (temp);
		str.erase(0, i);
		if (str[0] == ':')
		{
			if (paramLen == 4 && temp.size() == 3)
			{
				temp.push_back(str);
				return (temp);
			}
			str.erase(str.begin());
			temp.push_back(str);
			return (temp);
		}
		i = str.find_first_of(" ");
		if (i == std::string::npos)
		{
			temp.push_back(str);
			return (temp);
		}
		temp.push_back(str.substr(0, i));
		str.erase(0, i);
	}
	return (temp);
}

void	Server::selectCommand(int fd, std::string command)
{
	Client	*cl = getClient(fd);
	if (command.empty())
		return ;
	size_t	i;
	if (command[0] == ':')
	{
		i = command.find_first_of(" ");
		if (i != std::string::npos)
			command.erase(0, i);
	}
	if (command.size() > 1024)
		command.erase(1024, command.size() - 1024);
	
	std::vector<std::string>	args = splitCommand(command);
	i = command.find_first_not_of(" ");
	if (i != std::string::npos)
		command = command.substr(i);
	if (args.empty() || command.empty())
		return ;
	if (args.size() && (args[0] == "PASS"))
		authenticate(fd, command);
	else if (args.size() && (args[0] == "NICK"))
		setClientNick(fd, command);
	else if (args.size() && (args[0] == "USER"))
		setClientUser(fd, command);
	else if (args.size() && (args[0] == "QUIT"))
		quit(fd, command);
	else if (args.size() && (args[0] == "CAP" || args[0] == "WHO"))
		return ;
	else if (cl->getLogged())
	{
		if (args.size() && (args[0] == "JOIN"))
			join(fd, command);
		 else if (args.size() && (args[0] == "KICK"))
			kick(fd, command);
		else if (args.size() && (args[0] == "PRIVMSG" || args[0] == "privmsg"))
			privmsg(fd, command);
		else if (args.size() && (args[0] == "INVITE"))
			invite(fd, command);
		else if (args.size() && (args[0] == "MODE"))
			mode(fd, command);
		else if (args.size() && (args[0] == "TOPIC"))
			topic(fd, command);
		else if (args.size())
			sendToClient(fd, ERR_UNKNOWNCOMMAND(getClient(fd)->getNickname(), args[0]));	
	}
	else if (!cl->getLogged())
		sendToClient(fd, ERR_UNKNOWNCOMMAND(getClient(fd)->getNickname(), args[0])); // send message to the client. (do defines)
}

void	Server::sendToClient(int fd, std::string str)
{
	if (send(fd, str.c_str(), str.size(), 0) < 0)
		std::cerr << "Sending message to client failed." << std::endl;
}

void	Server::sendToClient(std::string str)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		sendToClient(_clients[i].getFd(), str);
	}
}

void	Server::sendToClient(Channel ch, std::string str)
{
	for (size_t i = 0; i < ch.get_operators().size(); i++)
		sendToClient(ch.get_operators()[i].getFd(), str);
	for (size_t i = 0; i < ch.get_users().size(); i++)
		sendToClient(ch.get_users()[i].getFd(), str);
}

void	Server::sendToClient(Channel ch, int fd,std::string str)
{
	for (size_t i = 0; i < ch.get_operators().size(); i++)
		if (ch.get_operators()[i].getFd() != fd)
			sendToClient(ch.get_operators()[i].getFd(), str);
	for (size_t i = 0; i < ch.get_users().size(); i++)
		if (ch.get_users()[i].getFd() != fd)
			sendToClient(ch.get_users()[i].getFd(), str);
}

void	Server::clearClient(int fd)
{
	closeFds(fd);
	for (size_t i = 0; i < _fds.size(); i++) // To remove client from pollfd
	{
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i);
			break ;
		}
	}
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			_clients.erase(_clients.begin() + i);
			break ;
		}
	}
}

void	Server::closeFds(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			std::cout << "Client <" << _clients[i].getFd() << "> Disconnected" << std::endl;
			close(_clients[i].getFd());
			break ;
		}
	}
}

void	Server::closeFds()
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::cout << "Client <" << _clients[i].getFd() << "> Disconnected" << std::endl;
		close(_clients[i].getFd());
	}
	if (_serSocketFd != -1)
	{
		std::cout << "Server <" << _serSocketFd << "> Disconnected" << std::endl;
		close(_serSocketFd);
	}
}

bool	Server::isRegistered(int fd)
{
	Client	*temp = getClient(fd);
	if (!temp || temp->getNickname().empty() || temp->getUsername().empty() || temp->getNickname() == "*" || !temp->getLogged())
		return (false);
	return (true);
}

Client	*Server::getClient(int fd)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getFd() == fd)
			return &this->_clients[i];
	}
	return (NULL);
}

int	Server::getClient(std::string name)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getNickname() == name)
			return this->_clients[i].getFd();
	}
	return (-1);
}

Client	*Server::getClientPtr(std::string name)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getNickname() == name)
			return &this->_clients[i];
	}
	return (NULL);
}

Channel	*Server::getChannel(std::string name)
{
	int	size = _channels.size();
	for (int i = 0; i < size; i++)
	{
		if (name == _channels[i].get_name())
			return &(_channels[i]);
	}
	return (NULL);
}

bool	Server::isOnServerClient(std::string name)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickname() == name)
			return (true);
	}
	return (false);
}

bool	Server::isOnServerChannel(std::string name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].get_name() == name)
			return (true);
	}
	return (false);
}
