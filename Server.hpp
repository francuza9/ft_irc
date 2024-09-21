#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include "Channel.hpp"

class Client;
class Channel;

class Server
{
	private:
		std::vector<Client>			_clients;
		std::vector<Channel>		_channels;
		std::vector<struct pollfd>	_fds;
		static bool					_endSignal;
		std::string					_password;
		int							_port;
		int							_serSocketFd;

	public:
		Server()	{_serSocketFd = -1;}

		// Server / Socket handling
		void		serverInit(int, std::string);
		void		serverSocket();
		void		acceptClient();
		void		recieveData(int);
		void		closeFds(int);
		void		closeFds();
		void		clearClient(int);
		static void	sigHandler(int);
	
		// Utils
		std::vector<std::string>	splitBuffer(std::string);
		std::vector<std::string>	splitCommand(std::string);
		std::vector<std::string>	getSplitParams(std::string, unsigned int paramLen);
		bool	isRegistered(int fd);
		void	sendToClient(int, std::string);
		void	sendToClient(std::string);
		void	sendToClient(Channel, std::string);
		void	sendToClient(Channel, int, std::string);
		bool	nickInUse(std::string);
		bool	isOnServerClient(std::string);
		bool	isOnServerChannel(std::string);
		int		getClient(std::string);
		Client		*getClientPtr(std::string);

		// Exec
		void	selectCommand(int fd, std::string command);
		void	authenticate(int, std::string);
		void	setClientNick(int, std::string);
		void	setClientUser(int, std::string);
		void	quit(int, std::string);
		void	join(int, std::string);
		void	mode(int, std::string);
		void	privmsg(int, std::string);
		void	invite(int, std::string);
		void	topic(int, std::string);
		void	kick(int, std::string);

		// Getters
		Client	*getClient(int fd);
		Channel	*getChannel(std::string name);
};

#endif