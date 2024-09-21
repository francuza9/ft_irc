#include "../Server.hpp"

#define SUCCESS		0
#define EMPTY_STR	1
#define NO_TEXT		2
#define MAX_KICKS	5

int		parseKick(std::string, std::vector<std::string> &, std::vector<std::string> &, std::string &);

void	Server::kick(int fd, std::string str)
{
	std::vector<std::string>	channels, users;
	std::string					comment = "";
	Client						*client = getClient(fd);
	Channel						*channel;
	int							errnum;

	errnum = parseKick(str, channels, users, comment); // parse

	if (errnum == EMPTY_STR || channels.size() < 1) // check if channels correspond to users (quantity)
	{
		sendToClient(fd, ERR_NEEDMOREPARAMS(client->getNickname(), std::string("KICK")));
		return ;
	}
	for (size_t i = 0; i < channels.size() && i < MAX_KICKS; i++) // check everything for each nick
	{
		channel = getChannel(channels[i]);
		for (size_t x = 0; x < users.size() && x < MAX_KICKS; x++)
		{
			if (!channel)
			{
				sendToClient(fd, ERR_NOSUCHCHANNEL(client->getNickname(), channels[i]));
				continue ;
			}
			if (!isOnServerClient(users[x]))
			{
				{
					sendToClient(fd, ERR_NOSUCHNICK(client->getNickname(), users[x]));
					continue ;
				}
			}
			if (!channel->is_member(*client))
			{
				sendToClient(fd, ERR_NOTONCHANNEL(client->getNickname(), channels[i]));
				continue ;
			}
			if (!channel->is_operator(*client))
			{
				sendToClient(fd, ERR_CHANOPRIVSNEEDED(client->getNickname(), channels[i]));
				continue ;
			}
			Client	*end = getClientPtr(users[x]);
			if (!channel->is_member(*end))
			{
				sendToClient(fd, ERR_NOSUCHNICK(client->getNickname(), channels[i]));
				continue ;
			}
			if (_channels[i].remove_member(*end) == 1)
					_channels.erase(_channels.begin() + i);

			sendToClient(*channel, RPL_KICKED(client->getID(), channel->get_name(), end->getNickname(), comment));
			sendToClient(end->getFd(), RPL_KICKED(client->getID(), channel->get_name(), end->getNickname(), comment));
		}
	}
}

void	Server::topic(int fd, std::string str)
{
	std::vector<std::string>	params = this->getSplitParams(str, 5);
	Client						*client = getClient(fd);
	Channel						*channel;

	if (params.empty()) // check if strings empty
	{
		sendToClient(fd, ERR_NEEDMOREPARAMS(client->getNickname(), std::string("INVITE")));
		return ;
	}
	channel = getChannel(params[0]);
	if (!channel) // check if channel exists
	{
		sendToClient(fd, ERR_NOSUCHCHANNEL(client->getNickname(), params[0]));
		return ;
	}
	if (!channel->is_member(*client)) // check if client is a member of the channel
	{
		sendToClient(fd, ERR_NOTONCHANNEL(client->getNickname(), params[0]));
		return ;
	}
	if (params.size() == 1) // check if client wants to see topic or modify it
	{
		if (!channel->get_topic().empty()) // check if topics empty
			sendToClient(fd, RPL_TOPIC(client->getNickname(), params[0], channel->get_topic())); // print topic
		else
			sendToClient(fd, RPL_NOTOPIC(client->getNickname(), params[0]));
	}
	else // client wants to modify topic
	{
		if (channel->is_topic_restrict() && !channel->is_operator(*client)) // if there's restriction and client is not operator send error
		{
			sendToClient(fd, ERR_CHANOPRIVSNEEDED(client->getNickname(), params[0]));
			return ;
		}
		channel->set_topic(params[1]); // change topic
		sendToClient(*channel, RPL_TOPICCHANGED(client->getID(), channel->get_name(), params[1]));
	}
}

void	Server::invite(int fd, std::string str)
{
	Channel						*channel;
	Client						*client = getClient(fd);
	std::vector<std::string>	params = this->getSplitParams(str, 6);

	if (params.empty() || params.size() == 1) // check if theres enough params
	{
		sendToClient(fd, ERR_NEEDMOREPARAMS(client->getNickname(), std::string("INVITE")));
		return ;
	}
	if (!isOnServerChannel(params[1])) // check if such channel exists
	{
		sendToClient(fd, ERR_NOSUCHCHANNEL(client->getNickname(), params[1]));
		return ;
	}
	if (!isOnServerClient(params[0])) // check if end client is on the server
	{
		sendToClient(fd, ERR_NOSUCHNICK(client->getNickname(), params[0]));
		return ;
	}
	channel = getChannel(params[1]);
	Client	*end = getClientPtr(params[0]);
	if (!channel->is_member(*client)) // check if client is on the channel
	{
		sendToClient(fd, ERR_NOTONCHANNEL(client->getNickname(), params[1]));
		return ;
	}
	if (!channel->is_operator(*client)) // check if client is operator
	{
		sendToClient(fd, ERR_CHANOPRIVSNEEDED(client->getNickname(), params[1]));
		return ;
	}
	if (channel->is_member(*end)) // check if end client is already on the channel 
	{
		sendToClient(fd, ERR_USERONCHANNEL(client->getNickname(), params[1], params[0]));
		return ;
	}
	if (!channel->is_invited(*end)) // check if end client is already invited
	{
		channel->add_to_invited(*end);
		sendToClient(fd, RPL_INVITING(client->getNickname(), params[1], params[0]));
		sendToClient(end->getFd(), RPL_INVITED(client->getID(), params[0], params[1]));
	}
}

int		parsePrivmsg(std::string, std::string &, std::vector<std::string> &);

void	Server::privmsg(int fd, std::string str)
{
	Channel						*temp;
	Client						*client = getClient(fd);
	int							errnum;
	std::string					msg;
	std::vector<std::string>	recievers, done;

	errnum = parsePrivmsg(str, msg, recievers);

	if (errnum == EMPTY_STR)
		sendToClient(fd, ERR_NORECIPIENT(client->getNickname(), std::string("PRIVMSG")));
	else if (errnum == NO_TEXT || msg.empty() || msg == ":")
		sendToClient(fd, ERR_NOTEXTTOSEND(client->getNickname()));
	if (errnum || msg.empty() || msg == ":")
		return ;
	for (size_t i = 0; i < recievers.size(); i++)
	{
		if (!done.empty() && std::find(done.begin(), done.end(), recievers[i]) != done.end())
			continue ;
		if (recievers[i][0] == '&' || recievers[i][0] == '#') // sending to channels
		{
			temp = getChannel(recievers[i]);
			if (!temp || !temp->is_member(*client))
				sendToClient(fd, ERR_CANNOTSENDTOCHAN(client->getNickname(), recievers[i]));
			else
				sendToClient(*temp, fd, RPL_PRIV(client->getID(), recievers[i], msg));
			
		}
		else  // sending to users
		{
			if (isOnServerClient(recievers[i]))
				sendToClient(getClient(recievers[i]), RPL_PRIV(client->getID(), client->getNickname(), msg));
			else
				sendToClient(fd, ERR_NOSUCHNICK(client->getNickname(), recievers[i]));
		}
		done.push_back(recievers[i]);
	}
}

int		parseKick(std::string str, std::vector<std::string> &channels, std::vector<std::string> &users, std::string &comment)
{
	size_t	i;
	bool	flag = true;

	str.erase(0,4);
	i = str.find_first_not_of(" ");
	if (i == std::string::npos)
		return (EMPTY_STR);  // spaces removed in the beggining
	str.erase(0, i);
	while (flag) // getting channels
	{
		i = str.find_first_of(", ");
		if (i == std::string::npos)
		{
			channels.push_back(str);
			return (SUCCESS);
		}
		channels.push_back(str.substr(0, i));
		if (str[i] == ' ')
			flag = false;
		str.erase(0, i + 1);
	}
	i = str.find_first_not_of(" ");
	if (i == std::string::npos)
		return (SUCCESS);
	str.erase(0, i);
	while (!flag) // getting users
	{
		i = str.find_first_of(", ");
		if (i == std::string::npos)
		{
			users.push_back(str);
			return (SUCCESS);
		}
		users.push_back(str.substr(0, i));
		if (str[i] == ' ')
			flag = true;
		str.erase(0, i + 1);
	}
	i = str.find_first_not_of(" ");
	if (i == std::string::npos)
		return (SUCCESS);
	str.erase(0, i);
	if (!str.empty())
	{
		if (str[0] == ':')
		{
			str.erase(str.begin());
			comment = str;
			return (SUCCESS);
		}
		i = str.find_first_of(" ");
		if (i == std::string::npos)
		{
			comment = str;
			return (SUCCESS);
		}
		comment = str.substr(0, i);
	}
	return (SUCCESS);
}

int		parsePrivmsg(std::string str, std::string &msg, std::vector<std::string> &vec)
{
	bool		flag = true;
	size_t		i;

	str.erase(0,7);
	i = str.find_first_not_of(" ");
	if (i == std::string::npos)
		return (EMPTY_STR);
	str.erase(0, i);  // remove the spaces before
	while (flag)
	{
		i = str.find_first_of(", ");
		if (i == std::string::npos)
		{
			vec.push_back(str);
			return (SUCCESS);
		}
		vec.push_back(str.substr(0, i));
		if (str[i] == ' ')
			flag = false;
		str.erase(0, i + 1);
	}
	i = str.find_first_not_of(" ");
	if (i == std::string::npos)
		return (NO_TEXT);
	str.erase(0, i);
	if (!str.empty())
	{
		if (str[0] == ':')
		{
			str.erase(str.begin());
			msg = str;
			return (SUCCESS);
		}
		i = str.find_first_of(" ");
		if (i == std::string::npos)
		{
			msg = str;
			return (SUCCESS);
		}
		msg = str.substr(0, i);
	}
	return (SUCCESS);
}
