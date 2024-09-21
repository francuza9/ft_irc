#include "Server.hpp"

// std::vector<std::string>	params = this->getSplitParams(str, 4); for parsing the arguments
// (4) is for length of the command itself (QUIT -> 4 chars)
// "params" is a vector that contains params.size() amount of parameters in order.
// Client	*cl = getClient(fd); -> returns a pointer to the current client object
// its needed for sending appropirate information/messages to them

void	Server::authenticate(int fd, std::string str)
{
	Client	*cl = getClient(fd);
	std::vector<std::string>	params = this->getSplitParams(str, 4);

	if (_password.empty() || _password.size() > 20)
		return ;
	if (!cl->getNickname().empty() || !cl->getUsername().empty())
	{
		sendToClient(fd, ERR_ALREADYREGISTRED(cl->getNickname(), std::string("EXIT")));
		return ;
	}
	else if (params.empty())
	{
		sendToClient(fd, ERR_NEEDMOREPARAMS(cl->getNickname(),std::string("EXIT"))); // If string is empty
		return ;
	}
	if (params[0] == _password)
		cl->setRegistered(true);
	else
		cl->setRegistered(false);
}
bool	wrongNick(std::string str);

void	Server::setClientNick(int fd, std::string str)
{
	std::vector<std::string>	params = this->getSplitParams(str, 4);
	std::string	current;
	Client	*cl = getClient(fd);
	
	if (!params.empty() && cl->getNickname() == params[0])
		return ;
	if (params.empty())
		sendToClient(fd, ERR_NONICKNAMEGIVEN(cl->getNickname(), std::string("NICK")));
	else if (wrongNick(params[0]))
		sendToClient(fd, ERR_ERRONEUSNICKNAME(cl->getNickname(), std::string("NICK")));
	else if (nickInUse(params[0]))
		sendToClient(fd, ERR_NICKNAMEINUSE(cl->getNickname(), std::string("NICK")));
	else
	{
		if (cl->getNickname().empty())
			cl->setNickname(params[0]);
		else
		{
			if (cl->getLogged())
				sendToClient(RPL_NICKCHANGED(cl->getID(), params[0]));
			cl->setNickname(params[0]);
		}
		if ((cl->getRegistered() || _password.empty()) && !cl->getUsername().empty() && !cl->getLogged())
		{
			sendToClient(fd, RPL_WELCOMEMSG(cl->getNickname()));
			cl->setLogged(true);
		}
	}
}

void	Server::setClientUser(int fd, std::string str)
{
	std::vector<std::string>	params = this->getSplitParams(str, 4);
	std::string	current;
	Client	*cl = getClient(fd);
	
	if (params.empty() || params.size() < 4)
		sendToClient(fd, ERR_NEEDMOREPARAMS(cl->getNickname(), std::string("USER")));
	else if (params[3][0] != ':')
		return ;
	else
	{
		if (params[1].find_first_of(":") != std::string::npos)
			return ;
		cl->setUsername(params[0]);
		cl->setHostname(params[1]);
		cl->setServername(params[2]);
		cl->setRealname(params[3]);
		if ((cl->getRegistered() || _password.empty()) && !cl->getNickname().empty() && !cl->getLogged())
		{
			sendToClient(fd, RPL_WELCOMEMSG(cl->getNickname()));
			cl->setLogged(true);
		}
	}
}

bool	wrongNick(std::string str)
{
	if (str.size() > 9)
		return (true);
	if (!isalpha(str[0]))
		return (true);
	for (size_t i = 0; str[i]; i++)
	{
		char c = str[i];
		if (isalnum(c))
			continue ;
		else if (c == '-' || c == '[' || c == ']' || c == '\\' ||
			c == '`' || c == '^' || c == '{' || c == '}')
			continue ;
		else
			return (true);
	}
	return (false);
}


bool	Server::nickInUse(std::string str)
{
	for (size_t i = 0; i < _clients.size(); i++)
		if (_clients[i].getNickname() == str)
			return (true);
	return (false);
}
