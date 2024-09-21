#include "Bot.hpp"

void	Bot::teachParse(int sock,std::string str,std::string sender)
{
	size_t	i;

	str.erase(0,1);
	i = str.find_first_of(":");
	if (i == std::string::npos)
		return ;	
	str.erase(0, i + 1);
	if (str == "BOT TEACH")
		teach(sock, sender);
	else
		sendPrivmsg(sock, ERR_TEACH, sender);
}

void	Bot::speakParse(int sock,std::string str,std::string sender)
{
	size_t	i;

	str.erase(0,1);
	i = str.find_first_of(":");
	if (i == std::string::npos)
		return ;	
	str.erase(0, i + 1);
	if (str == "BOT SPEAK GEO")
		speak(sock, sender, std::string("GEO"));
	else if (str == "BOT SPEAK RUS")
		speak(sock, sender, std::string("RUS"));
	else
		sendPrivmsg(sock, ERR_SPEAK, sender);
}

void	Bot::hymnParse(int sock,std::string str,std::string sender)
{
	size_t	i;

	str.erase(0,1);
	i = str.find_first_of(":");
	if (i == std::string::npos)
		return ;	
	str.erase(0, i + 1);
	if (str == "BOT HYMN GEO")
		hymn(sock, sender, std::string("GEO"));
	else if (str == "BOT HYMN RUS")
		hymn(sock, sender, std::string("RUS"));
	else
		sendPrivmsg(sock, ERR_HYMN, sender);
}

int	Bot::getLanguage(std::string str,std::string &lang)
{
	size_t	i = 0;

	str.erase(0,1);
	i = str.find_first_of(":");
	if (i != std::string::npos)
	str.erase(0, i + 1);
	if (str == "BOT SET GEO")
		lang = "Georgian";
	else if (str == "BOT SET RUS")
		lang = "Russian";
	else if (str == "BOT SET STOP")
		lang = "";
	else
		return (1);
	return (0);
}
void		Bot::inviteParse(int fd, std::string str)
{
	str.erase(0, 1);
	size_t i = str.find_first_of(":");
	str.erase(0,i + 1);
	if (str.find_first_of(" ") == std::string::npos
		&& (str[0] == '#' || str[0] == '&'))
		inviteAccept(fd, str);
}

std::string	Bot::getSender(std::string str)
{
	size_t	i;

	i = str.find_first_of(" ");
	str.erase(0, i + 1);
	i = str.find_first_of(" ");
	str.erase(0, i + 1);
	i = str.find_first_of(" ");
	return (str.substr(0, i));
}

void	Bot::getInput(std::string str, std::string &input)
{
	str.erase(0,1);
	size_t i;
	i = str.find_first_of(":");
	str.erase(0, i + 1);
	input = str;
}

void Bot::sendMessage(int fd, std::string msg)
{
	if(send(fd, msg.c_str(), msg.size(), 0) == -1)
		std::cerr << "Send failed" << std::endl;
}

void Bot::sendPrivmsg(int sock, std::string mssg, std::string end)
{
	std::string msg = "PRIVMSG " + end + " :" + mssg + "\r\n";
	if (send(sock, msg.c_str(), msg.size(),0) == -1)
		std::cerr << "Send failed" << std::endl;
}