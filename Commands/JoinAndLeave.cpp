#include "../Server.hpp"

#define SUCCESS		0
#define EMPTY_STR	1
#define BAD_CHNAME	2

#define NO_SIGN					1
#define UNKNOWN_MODE			2
#define WRONG_CHANNEL			3
#define NO_MODEGIVEN			4
#define NO_CHANNELGIVEN			5
#define EMPTY_STRN				6

int	modeParse(std::string, std::string &, bool &, char &, std::vector<std::string> &);

bool	is_valid_limit(std::string);

void	Server::mode(int fd, std::string command)
{
	Client	*client = getClient(fd);
	bool						sign;
	char						mode;
	std::string					channel = "", paramsJoined = "";
	std::vector<std::string>	params;
	int							errnum;
	
	errnum = modeParse(command, channel, sign, mode, params);
	if (!channel.empty() && (mode == ' ' || mode == '\r' || mode == '\n'))
	{
		std::string	modes_s = "";
		Channel	*channel_to_modify =  getChannel(channel);

		
		if (channel_to_modify == NULL || !channel_to_modify->is_member(*client))
		{
			sendToClient(fd, ERR_NOTONCHANNEL(client->getNickname(), channel));
			return ;
		}
		if (channel_to_modify->is_invite_only())
			modes_s += 'i';
		if (channel_to_modify->is_topic_restrict())
			modes_s += 't';
		if (channel_to_modify->is_key_set())
			modes_s += 'k';
		if (channel_to_modify->is_limit_set())
			modes_s += 'l';
		if (!modes_s.empty())
			modes_s = "+" + modes_s;
		sendToClient(fd, RPL_CHANNELMODEIS(client->getNickname(), channel, modes_s));
		return ;
	}

	// make sure that if mode is given with only 1 parameter, print current modes on that
	// channel if client's inside
	// careful with order of errors

	//sendToClient(fd, RPL_CHANNELMODEIS(client->getNickname(), channel, mode, paramsJoined));


	if (errnum == EMPTY_STR)
		sendToClient(fd, ERR_NEEDMOREPARAMS(client->getNickname(), std::string("MODE")));
	else if (errnum)
		sendToClient(fd, ERR_UNKNOWNMODE(client->getNickname(), mode));
	if (errnum)
		return ;
	if (!params.empty())
		for (size_t i = 0; i < params.size(); i++)
			paramsJoined += params[i] + " ";
	
	Channel	*channel_to_modify =  getChannel(channel);
	if (channel_to_modify == NULL || !channel_to_modify->is_member(*client))
	{
		sendToClient(fd, ERR_NOTONCHANNEL(client->getNickname(), channel));
		return ;
	}
	if (!channel_to_modify->is_operator(*client))
	{
		sendToClient(fd, ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
		return ;
	}
	switch (mode)
	{
		case 'i':
			if (sign != channel_to_modify->is_invite_only())
			{
				channel_to_modify->toggle_invite_only();
				sendToClient(*channel_to_modify, RPL_CHANNELMODECHANGED(client->getID(), channel, (sign? '+' : '-'),mode, std::string("")));
			}
			break;

		case 't':
			if (sign != channel_to_modify->is_topic_restrict())
			{
				channel_to_modify->toggle_topic_restrict();
				sendToClient(*channel_to_modify, RPL_CHANNELMODECHANGED(client->getID(), channel, (sign? '+' : '-'),mode, std::string("")));
			}
			break;

		case 'k':
			if (!sign && channel_to_modify->is_key_set())
			{
				channel_to_modify->toggle_key_set();
				channel_to_modify->set_key("");
				if (params.empty())
					params.push_back("");
				sendToClient(*channel_to_modify, RPL_CHANNELMODECHANGED(client->getID(), channel, (sign? '+' : '-'),mode, std::string("")));
			}
			else if (sign)
			{
				if (params.empty())
					return ;
				if (channel_to_modify->check_key(params[0]))
					return ;
				if (!channel_to_modify->is_key_set())
					channel_to_modify->toggle_key_set();
				channel_to_modify->set_key(params[0]);
				sendToClient(*channel_to_modify, RPL_CHANNELMODECHANGED(client->getID(), channel, (sign? '+' : '-'),mode, params[0]));
			}
			break;

		case 'o':
			for (size_t i = 0; i < params.size() && i < 3; i++)
			{
				Client	*client_to_modify = getClientPtr(params[i]);
				if (client_to_modify == NULL || !channel_to_modify->is_member(*client_to_modify))
				{
					sendToClient(fd, ERR_NOSUCHNICK(client->getNickname(), channel));	
					continue;
				} 
				if (sign == channel_to_modify->is_operator(*client_to_modify))
				{
					//sendToClient(fd, ERR_ALREADYSET(client->getNickname(), channel));
					continue;
				}
				if (sign)
					channel_to_modify->set_to_operator(*client_to_modify);
				else
					channel_to_modify->set_to_user(*client_to_modify);
				sendToClient(*channel_to_modify, RPL_CHANNELMODECHANGED(client->getID(), channel, (sign? '+' : '-'),mode, params[i]));
			}
			break;

		case 'l':
			
			if (sign && params.size() > 0)
			{
				int limit = atoi(params[0].c_str());
				if (limit == 0)
					sign = false;
			}
			if (sign == false && channel_to_modify->is_limit_set())
			{
				channel_to_modify->toggle_limit_set();
				if (params.empty())
					params.push_back("");
				sendToClient(*channel_to_modify, RPL_CHANNELMODECHANGED(client->getID(), channel, (sign? '+' : '-'),mode, params[0]));
			}
			else if (sign)
			{
				if (params.size() == 0)
					return ;
				int	limit = atoi(params[0].c_str());
				if (channel_to_modify->get_user_limit() == limit)
					return ;
				channel_to_modify->set_user_limit(limit);
				if (!channel_to_modify->is_limit_set())
					channel_to_modify->toggle_limit_set();
				sendToClient(*channel_to_modify, RPL_CHANNELMODECHANGED(client->getID(), channel, (sign? '+' : '-'),mode, SSTR(limit)));
			}
			break;

		default:
			break;
	}
}

int	joinParse(std::string, std::vector<std::string> &, std::vector<std::string> &);

void	Server::join(int fd, std::string command)
{
	Client	*client = getClient(fd);
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;
	int							errnum;
	
	// fills up "channels" and "keys" vectors with channel names and passwords
	// returns error code in errrnum (0 == no error)
	// if theres errnum == 1 or == 2 send appropriate error message (i will create the messages)
	// channels and keys correspond in order 1:1 2:2
	errnum = joinParse(command, channels, keys);

	if (errnum == EMPTY_STR || channels.size() == 0)
	{
		sendToClient(fd, ERR_NEEDMOREPARAMS(client->getNickname(), std::string("JOIN")));
		return ;
	}

	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].size() > 200)
			channels[i].erase(200, channels[i].size() - 200);
		if (client->isMaxChannels())
		{
			sendToClient(fd, ERR_TOOMANYCHANNELS(client->getNickname(), channels[i]));
			continue ;
		}
		Channel	*to_join = getChannel(channels[i]);
		if (to_join == NULL)
		{
			if (channels[i][0] != '#' && channels[i][0] != '&')
			{
				sendToClient(fd, ERR_BADCHANMASK(client->getNickname(), std::string("JOIN")));
				continue ;
			}
			Channel	new_channel(*client, channels[i], keys[i]);
			_channels.push_back(new_channel);
			client->incrementChannel();
			sendToClient(new_channel, RPL_JOINCHAN(client->getID(), new_channel.get_name()));
			sendToClient(fd, RPL_JOINWHOSTART(client->getNickname(), new_channel.get_name(), new_channel.get_members_str()));
			sendToClient(fd, RPL_JOINWHOEND(client->getNickname(), new_channel.get_name()));
		}
		else
		{
			if (to_join->is_member(*client))
				continue;
			if (to_join->is_invite_only() && !to_join->is_invited(*client))
			{
				sendToClient(fd, ERR_INVITEONLYCHAN(client->getNickname(), channels[i]));
				continue;
			}
			if (to_join->is_limit_set() && (int)to_join->get_size() >= to_join->get_user_limit())
			{
				sendToClient(fd, ERR_CHANNELISFULL(client->getNickname(), channels[i])); 
				continue;
			}
			if (to_join->is_key_set() && !to_join->check_key(keys[i]))
			{
				sendToClient(fd, ERR_BADCHANNELKEY(client->getNickname(), channels[i]));
				continue;
			}
			to_join->add_user(*client);
			client->incrementChannel();
			sendToClient(*to_join, RPL_JOINCHAN(client->getID(), to_join->get_name()));
			sendToClient(fd, RPL_JOINWHOSTART(client->getNickname(), to_join->get_name(), to_join->get_members_str()));
			sendToClient(fd, RPL_JOINWHOEND(client->getNickname(), to_join->get_name()));
		}
	}

}

void	Server::quit(int fd, std::string command)
{
	std::vector<std::string>	params = this->getSplitParams(command, 4);
	std::vector<Channel>		temp;

	Client	*cl = getClient(fd);
	std::string	quitMSG = "", tempNick = cl->getID();

	// Removes client from every channel's every list and sends message.
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].is_member(*cl))
			if (_channels[i].remove_member(*cl) == 1)
					_channels.erase(_channels.begin() + i);
		if (_channels[i].is_invited(*cl))
			_channels[i].remove_member(*cl);
	}

	// Remove the client from the server.
	clearClient(fd);

	// Send message to everyone
	if (params.size() > 0)
		sendToClient(RPL_CLIENTQUIT(tempNick, params[0]));
	else
		sendToClient(RPL_CLIENTQUIT(tempNick, std::string("")));
}

int	modeParse(std::string str, std::string &channel, bool &sign, char &mode, std::vector<std::string> &params)
{
	size_t	i;

	mode = ' ';
	str.erase(0,4);
	i = str.find_first_not_of(" ");
	if (i == std::string::npos)
		return (EMPTY_STR);  // spaces removed in the beggining
	str.erase(0, i);
	i = 0;
	if (str[i] != '&' && str[i] != '#')
		return (WRONG_CHANNEL);
	i = str.find_first_of(" ");
	if (i == std::string::npos)
	{
		channel = str;
		return (SUCCESS);
	}
	channel = str.substr(0, i);
	str.erase(0, i);
	i = str.find_first_not_of(" ");
	if (i == std::string::npos)
		return (NO_MODEGIVEN);
	str.erase(0, i);
	i = 0;
	if (str[i] == '+')  // check if sign is given with the mode
		sign = true;
	else if (str[i] == '-')
		sign = false;
	else
	{
		mode = str[i];
		return (NO_SIGN);
	}
	str.erase(0,1);
	if (str.find_first_of("itkol") != 0)
	{
		mode = str[i];
		return (UNKNOWN_MODE);
	}
	mode = str[i];
	str.erase(0, 1);
	if (!str.empty() && str[0] != ' ')
	{
		mode = str[i];
		return (UNKNOWN_MODE);
	}
	while (!str.empty())
	{
		i = str.find_first_not_of(" ");
		if (i == std::string::npos)
			return (SUCCESS);
		str.erase(0, i);
		i = str.find_first_of(" ");
		if (i == std::string::npos)
		{
			params.push_back(str);
			return (SUCCESS);
		}
		params.push_back(str.substr(0, i));
		str.erase(0, i);
	}
	return (SUCCESS);
}

int	joinParse(std::string str, std::vector<std::string> &names, std::vector<std::string> &keys)
{
	size_t						i;
	bool						channelNames = true;

	str.erase(0,4);
	i = str.find_first_not_of(" ");
	if (i == std::string::npos)
		return (EMPTY_STRN);
	str.erase(0, i);  // remove the spaces before #channelnames start
	while (channelNames)
	{
		i = str.find_first_of(", ");
		if (i == std::string::npos)
		{
			names.push_back(str);
			while (names.size() > keys.size()) // to avoid segfault, adding extra no passwords to keys vector
				keys.push_back("");
			return (SUCCESS);
		}
		names.push_back(str.substr(0, i));
		if (str[i] == ' ')
			channelNames = false;
		str.erase(0, i + 1);
	}
	i = str.find_first_not_of(" ");
	if (i == std::string::npos)
	{
		while (names.size() > keys.size()) // to avoid segfault, adding extra no passwords to keys vector
			keys.push_back("");
		return (SUCCESS);
	}
	str.erase(0, i);
	while (!str.empty())
	{
		i = str.find_first_of(",");
		if (i == std::string::npos)
		{
			keys.push_back(str);
			while (names.size() > keys.size()) // to avoid segfault, adding extra no passwords to keys vector
				keys.push_back("");
			return (SUCCESS);
		}
		keys.push_back(str.substr(0, i));
		str.erase(0, i + 1);
	}
	while (names.size() > keys.size()) // to avoid segfault, adding extra no passwords to keys vector
		keys.push_back("");
	return (SUCCESS);
}

