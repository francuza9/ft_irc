#include "Client.hpp"

Client::Client()
{
	this->_nickname = "";
	this->_realname = "";
	this->_username = "";
	this->_buffer = "";
	this->_IPAddr = "";
	this->_fd = -1;
	this->_isOperator = false;
	this->_isLoggedIn = false;
	this->_isRegistered = false;
	this->_maxChannels = 100;
	this->_currentChannels = 0;
}
Client::Client(std::string nickname, std::string username, int fd)
{
	this->_nickname = nickname;
	this->_realname = "";
	this->_username = username;
	this->_buffer = "";
	this->_IPAddr = "";
	this->_fd = fd;
	this->_isOperator = false;
	this->_isLoggedIn = false;
	this->_isRegistered = false;
	this->_maxChannels = 100;
	this->_currentChannels = 0;
}

Client	&Client::operator=(const Client &x)
{
	if (this != &x)
	{
		this->_nickname = x._nickname;
		this->_realname = x._realname;
		this->_username = x._username;
		this->_buffer = x._buffer;
		this->_IPAddr = x._IPAddr;
		this->_fd = x._fd;
		this->_isOperator = x._isOperator;
		this->_isLoggedIn = x._isLoggedIn;
		this->_isRegistered = x._isRegistered;
		this->_channelInvites = x._channelInvites;
		this->_maxChannels = x._maxChannels;
		this->_currentChannels = x._currentChannels;
	}
	return (*this);
}

bool	Client::operator==(const Client &x)
{
	if (this != &x)
	{
		if (_fd == x._fd)
			return (true);
		return (false);
	}
	return (true);
}

void	Client::addChannelInvite(std::string name)
{
	_channelInvites.push_back(name);
}

void	Client::removeChannelInvite(std::string name)
{
	for (size_t i = 0; i < _channelInvites.size(); i++)
	{
		if (_channelInvites[i] == name)
		{
			_channelInvites.erase(_channelInvites.begin() + i);
			return ;
		}
	}
}

bool	Client::getChannelInvite(std::string str) const
{
	for (size_t i = 0; i < _channelInvites.size(); i++)
	{
		if (str == _channelInvites[i])
			return (true);
	}
	return (false);
}

std::string	Client::getID() const
{
	std::string x = _nickname + "!" + _username + "@" + _IPAddr;
	return (x);
}
