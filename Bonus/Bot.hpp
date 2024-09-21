#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <vector>
# include <cstdlib>
# include <netdb.h>
# include <sstream>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fstream>
# include <cstring>
# include <csignal>

# define ADDRESS	0
# define PORT		1
# define PASSWORD	2
# define NICKNAME	3

# define ERR_GENERALHLEP	"Unknown command: for list of commands --> \"BOT HELP\""
# define RPL_CMDS	"Available commands: SET, TEACH, SPEAK, HYMN, QUIT\r\n"
# define RPL_SET	"Usage of SET explained below.\r\n"
# define ERR_SET	"\"BOT SET <language>\"     language --> GEO | RUS | STOP\r\n"
# define RPL_TEACH	"Usage of TEACH explained below.\r\n"
# define ERR_TEACH	"\"BOT TEACH\" With no Arguments.\r\n"
# define RPL_SPEAK	"Usage of SPEAK explained below.\r\n"
# define ERR_SPEAK	"\"BOT SPEAK <language>\"     language --> GEO | RUS\r\n"
# define RPL_HYMN	"Usage of HYMN explained below.\r\n"
# define ERR_HYMN	"\"BOT HYMN <language>\"     language --> GEO | RUS\r\n"
# define RPL_QUIT	"Usage of QUIT explained below.\r\n"
# define ERR_QUIT	"\" * BOT HYMN * \"  (*) --> wildcard \r\n"


class	Bot
{
	public:
		Bot();
		Bot(const Bot &);
		Bot	&operator=(const Bot &);
		~Bot();
	
		void	init(int sock);

		void	setNick(std::string);

		std::string	getSender(std::string);
		void		getInput(std::string, std::string &);

		void	sendMessage(int, std::string);
		void	sendPrivmsg(int, std::string, std::string);
		

		// COMMANDS
		std::string	translate(std::string, std::string);
		void		teach(int, std::string);
		void		speak(int, std::string, std::string);
		void		hymn(int, std::string, std::string);
		void		help(int, std::string);
		void		quit(int);
		void		inviteAccept(int, std::string);
		
		// UTILS
		int			getLanguage(std::string,std::string &);
		std::string	translateGeo(std::string);
		std::string	translateRus(std::string);
		void		teachParse(int, std::string, std::string);
		void		speakParse(int, std::string, std::string);
		void		hymnParse(int, std::string, std::string);
		void		inviteParse(int, std::string);

	private:
		std::string	_nickname;
		bool		_registered;
};

#endif