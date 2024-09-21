#ifndef INCLUDES_HPP
# define INCLUDES_HPP

# include <iostream>
# include <vector> //-> for vector
# include <algorithm>
# include <sys/socket.h> //-> for socket()
# include <sys/types.h> //-> for socket()
# include <netinet/in.h> //-> for sockaddr_in
# include <fcntl.h> //-> for fcntl()
# include <unistd.h> //-> for close()
# include <arpa/inet.h> //-> for inet_ntoa()
# include <poll.h> //-> for poll()
# include <csignal> //-> for signal()
# include <cstring>
# include <cstdlib>
# include <sstream>

#	define SSTR(x) (static_cast< std::ostringstream &>((std::ostringstream() << std::dec << x ) )).str()

#	define CRLF			"\r\n"
#	define SERV			":irc_server"
#	define WHITESPACES	" \t\r\n\v\f"
#	define CHSTRING_DLM	" \a\0\r\n,"

#	define GEO	0
#	define RUS	1

// ----------------------- REPLIES ----------------------- //
# define RPL_NICKCHANGED(id, nick)						(":" + id + " NICK :" + nick + CRLF)
# define RPL_WELCOMEMSG(nick)							(":irc_server 001 " + nick + " :Welcome to the irc_server Network" + CRLF)
# define RPL_CLIENTQUIT(id, message)					(":" + id + " QUIT :" + message + CRLF)

// JOIN / TOPIC
# define RPL_JOIN(id, channel, realname)				(":" + id + " JOIN " + channel + " * :" + realname + CRLF)
# define RPL_JOINCHAN(id, channel)						(":" + id + " JOIN :" + channel + CRLF)
# define RPL_JOINWHOSTART(nick, channel, id)			(":irc_server 353 " + nick + " = " + channel + " :" + id + CRLF)
# define RPL_JOINWHOEND(nick, channel)					(":irc_server 366 " + nick + " " + channel + " :End of /NAMES list." + CRLF)
# define RPL_TOPIC(nick, channel, topic)				(":irc_server 332 " + nick + " " + channel + " :" + topic + CRLF)
# define RPL_NOTOPIC(nick, channel)						(":irc_server 331 " + nick + " " + channel + " :No topic is set." + CRLF)
# define RPL_TOPICCHANGED(id, channel, msg)				(":" + id + " TOPIC " + channel + " :" + msg + CRLF)

// MODE
# define RPL_CHANNELMODEIS(nick, channel, every_mode) 				(":irc_server 324 " + nick + " " + channel + " " + every_mode + CRLF)
# define RPL_CHANNELMODECHANGED(id, channel, sign, mode, end) 		(":"+ id + " MODE " + channel + " " + sign + mode + " " + end + CRLF)

// INVITE
# define RPL_INVITING(nick, channel, invited)						(":irc_server 341 " + nick + " " + invited + " " + channel + CRLF)
# define RPL_INVITED(id, end, channel)								(":" + id + " INVITE " + end + " :" + channel + CRLF)

// PRIVMSG
# define RPL_PRIV(id, end, msg)										(":" + id + " PRIVMSG " + end + " :" + msg + CRLF)

// KICK
# define RPL_KICKED(id, channel, end, msg)							(":" + id + " KICK " + channel + " " + end + " :" + msg + CRLF)


// ----------------------- ERRORS ----------------------- //
// AUTHENTICATION
# define ERR_NEEDMOREPARAMS(nick, command)				(":irc_server 461 " + nick + " " + command + " :Not enough parameters" + CRLF)
# define ERR_ALREADYREGISTRED(nick, command)			":irc_server 462 " + nick + " :You may not reregister\r\n"
# define ERR_NONICKNAMEGIVEN(nick, command)				":irc_server 431 " + nick + " " + command + " :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME(nick, badnick)			(":irc_server 432 " + nick + " " + badnick + " :Erroneus nickname" + CRLF)
# define ERR_NICKNAMEINUSE(nick, badnick)				(":irc_server 433 " + nick + " " + badnick + " :Nickname is already in use" + CRLF)
# define ERR_ERRONEUSUSERNAME(nick, command)			(":irc_server " + nick + " " + command + " :Erroneus username" + CRLF)
# define ERR_ERRONEUSHOSTNAME(nick, command)			(":irc_server " + nick + " " + command + " :Erroneus hostname" + CRLF)
# define ERR_ERRONEUSSERVERNAME(nick, command)			(":irc_server " + nick + " " + command + " :Erroneus servername" + CRLF)
# define ERR_UNKNOWNCOMMAND(nick, command)				(":irc_server 421 " + nick + " " + command + " :Unknown command" + CRLF)

// JOIN
# define ERR_BADCHANMASK(nick, command)					(":irc_server 476 " + nick + " " + command + " :Bad channel name" CRLF)
# define ERR_INVITEONLYCHAN(nick, channel) 				(":irc_server 473 " + nick + " " + channel + " :Cannot join channel (+i)" + CRLF)
# define ERR_CHANNELISFULL(nick, channel) 				(":irc_server 471 " + nick + " " + channel + " :Cannot join channel (+l)" + CRLF)
# define ERR_BADCHANNELKEY(nick, channel)				(":irc_server 475 " + nick + " " + channel + " :Cannot join channel (+k)" + CRLF)
# define ERR_NOSUCHCHANNEL(nick, channel)				(":irc_server 403 " + nick + " " + channel + " :No such channel" + CRLF)
# define ERR_TOOMANYCHANNELS(nick, channel)				(":irc_server 405 " + nick + " " + channel + " :You have joined too many channels" + CRLF)

// MODE
# define ERR_CHANOPRIVSNEEDED(nick, channel)			(":irc_server 482 " + nick + " " + channel + " :You're not channel operator" + CRLF)
# define ERR_NOTONCHANNEL(nick, channel)				(":irc_server 442 " + nick + " " + channel + " :You're not on that channel" + CRLF)
# define ERR_UNKNOWNMODE(nick, param)					(":irc_server 472 " + nick + " " + param + " :is unknown mode char to me" + CRLF)
# define ERR_KEYSET(nick, channel)						(":irc_server 467 " + nick + " " + channel + " :Channel key already set" + CRLF)
# define ERR_INVALIDLIMIT(nick, channel)				(":irc_server " + nick + " " + channel + " :Cannot set this limit" + CRLF)
# define ERR_ALREADYSET(nick, channel)					(":irc_server " + nick + " " + channel + " :Mode already set" + CRLF)

// PRIVMSG
# define ERR_NORECIPIENT(nick, command)					(":irc_server 411 " + nick + " :No recipient given (" + command + ")" + CRLF)
# define ERR_CANNOTSENDTOCHAN(nick, channel)			(":irc_server 404 " + nick + " " + channel + " :Cannot send to channel" + CRLF)
# define ERR_NOSUCHNICK(nick, recipient)				(":irc_server 401 " + nick + " " + recipient + " :No such nick/channel" + CRLF)
# define ERR_NOTEXTTOSEND(nick)							(":irc_server 412 " + nick + " :No text to send" + CRLF)

// INVITE
# define ERR_USERONCHANNEL(nick, channel, invited)		(":irc_server 443 " + nick + " " + invited + " " + channel + " :is already on channel" + CRLF)

#endif