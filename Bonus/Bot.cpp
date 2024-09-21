#include "Bot.hpp"

Bot::Bot() {_nickname = ""; _registered = false;}
Bot::Bot(const Bot &x) {*this = x;};
Bot::~Bot() {};
Bot	&Bot::operator=(const Bot &x)
{
	if (this != &x)
	{
		_nickname = x._nickname;
		_registered = x._registered;
	}
	return (*this);
}

void Bot::init(int sock)
{
	std::string recieved, language, command;
	ssize_t 	recivedBytes;
	int			errnum;

	char buff[1024];
	while(true)
	{
		memset(buff, 0, sizeof(buff));
		recivedBytes = recv(sock, buff, (sizeof(buff) - 1), 0);
		if(recivedBytes <= 0)
		{
			close(sock);
			break;
		}
		recieved = buff;
		size_t pos = recieved.find_first_of("\n\r");
		if(pos != std::string::npos)
			recieved = recieved.substr(0, pos);
		if(recieved == ":irc_server 001 " + _nickname + " :Welcome to the irc_server Network" && !_registered)
		{
			_registered = true;
			sendMessage(sock, "JOIN #botChannel1,#botChannel2,#botChannel3\r\n");
		}
		else if (!_registered)
		{
			close(sock);
			std::cout << recieved << std::endl;
			return ;
		}
		else if(recieved.find("PRIVMSG") != std::string::npos && _registered)
		{
			if (recieved.find("BOT SET") != std::string::npos)
			{
				errnum = getLanguage(recieved, language);
				if (errnum)
				{
					sendPrivmsg(sock, ERR_SET, getSender(recieved));
					continue ;
				}
			}
			else if (recieved.find("BOT HELP") != std::string::npos)
				help(sock, recieved);
			else if (recieved.find("BOT TEACH") != std::string::npos)
				teachParse(sock, recieved, getSender(recieved));
			else if (recieved.find("BOT SPEAK") != std::string::npos)
				speakParse(sock, recieved, getSender(recieved));
			else if (recieved.find("BOT HYMN") != std::string::npos)
				hymnParse(sock, recieved, getSender(recieved));
			else if (recieved.find("BOT QUIT") != std::string::npos)
				quit(sock);
			else if (recieved.find("BOT") != std::string::npos)
				sendPrivmsg(sock, ERR_GENERALHLEP, getSender(recieved));
			else if (!language.empty())
			{
				std::string translation;
				getInput(recieved, command);
				translation = translate(command, language);
				sendPrivmsg(sock, translation, getSender(recieved));
			}
		}
		else if (recieved.find("INVITE") != std::string::npos)
			inviteParse(sock, recieved);
	}
}

void	Bot::setNick(std::string str)
{
	_nickname = str;
}
