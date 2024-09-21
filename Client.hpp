#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Channel.hpp"
# include "Server.hpp"

class Client
{
	public:
		Client();
		Client(std::string nickname, std::string username, int fd);
		Client(const Client &x) {*this = x;}
		Client	&operator=(const Client &);
		bool	operator==(const Client &x);
	
		std::string	getIPAddr() const					{return (_IPAddr);}
		int			getFd() const						{return (_fd);}
		std::string	getUsername() const					{return (_username);}
		std::string	getNickname() const					{return (_nickname);}
		std::string	getRealname() const					{return (_realname);}
		std::string	getBuffer() const					{return (_buffer);}
		bool		getOperator() const					{return (_isOperator);}
		bool		getRegistered() const				{return (_isRegistered);}
		bool		getLogged() const					{return (_isLoggedIn);}
		bool		getChannelInvite(std::string) const;
		std::string	getID() const;

		void		setFd(int fd)				{_fd = fd;}
		void		setIPAddr(std::string x)	{_IPAddr = x;}
		void		setUsername(std::string &x)	{_username = x;}
		void		setNickname(std::string &x)	{_nickname = x;}
		void		setRealname(std::string &x)	{_realname = x;}
		void		setBuffer(std::string x)	{_buffer += x;}
		void		setOperator(bool x)			{_isOperator = x;}
		void		setRegistered(bool x)		{_isRegistered = x;}
		void		setLogged(bool x)			{_isLoggedIn = x;}
		void		setHostname(std::string x)	{_hostname = x;}
		void		setServername(std::string x)	{_servername = x;}

		void		addChannelInvite(std::string);
		void		removeChannelInvite(std::string);
		void		clearBuffer()				{_buffer.clear();}
		void		incrementChannel()			{_currentChannels++;}
		bool		isMaxChannels()				{return (_currentChannels >= _maxChannels);}

	private:
		std::string					_IPAddr;
		int							_fd;

		std::vector<std::string>	_channelInvites;
		std::string					_username; // not unique 
		std::string					_nickname; // unique		special identifier: nickname!username@ipaddress
		std::string					_hostname;
		std::string					_servername;
		std::string					_realname; // not unique
		std::string					_buffer;

		unsigned int				_maxChannels;
		unsigned int				_currentChannels;

		bool						_isOperator;
		bool						_isRegistered;
		bool						_isLoggedIn;

};
#endif
