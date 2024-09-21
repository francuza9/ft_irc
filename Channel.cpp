#include "Channel.hpp"

/*------------------------------CONSTRUCTORS/DESTRUCTOR------------------------------*/

Channel::Channel(Client channel_creator, std::string name, std::string key) : _name(name), _key(key)
{
	_invite_only = false;
	_topic_restrict = false;
	_key_set = true;
	if (key.empty())
		_key_set = false;
	_limit_set = false;

	_topic = "";
	_size = 1;
	_user_limit = 0;

	_operators.push_back(channel_creator);
}

Channel::Channel(Channel const &src)
{
	*this = src;
}

Channel	&Channel::operator=(Channel const &rhs)
{
	if (this != &rhs)
	{
		_name = rhs._name;
		_key = rhs._key;
		_topic = rhs._topic;
		_user_limit = rhs._user_limit;
		_size = rhs._size;
		_invite_only = rhs._invite_only;
		_topic_restrict = rhs._topic_restrict;
		_key_set = rhs._key_set;
		_limit_set = rhs._limit_set;
		_users = rhs._users;
		_operators = rhs._operators;
		_invited = rhs._invited;
	}
	return (*this);
}

Channel::~Channel() {};

std::vector<Client>	Channel::get_operators() const 
{
	return (_operators);
}

/*------------------------------ACCESSORS------------------------------*/


std::vector<Client>	Channel::get_users() const 
{
	return (_users);
}

std::vector<Client>	Channel::get_invited() const 
{
	return (_invited);
}

bool	Channel::is_invite_only(void) const
{
	return (_invite_only);
}

bool	Channel::is_topic_restrict(void) const
{
	return (_topic_restrict);
}

bool	Channel::is_key_set(void) const
{
	return (_key_set);
}

bool	Channel::is_limit_set(void) const
{
	return (_limit_set);
}

int		Channel::toggle_invite_only(void)
{
	_invite_only = !_invite_only;
	return (0);
}

int		Channel::toggle_topic_restrict(void)
{
	_topic_restrict = !_topic_restrict;
	return (0);
}

int		Channel::toggle_key_set(void)
{
	_key_set = !_key_set;
	return (0);
}

int		Channel::toggle_limit_set(void)
{
	_limit_set = !_limit_set;
	return (0);
}

int	Channel::get_user_limit(void) const
{
	return (_user_limit);
}

std::string		Channel::get_topic(void) const
{
	return (_topic);
}
std::string		Channel::get_key(void) const
{
	return (_key);
}

int		Channel::set_topic(std::string topic)
{
	_topic = topic;
	return (0);
}

int		Channel::set_key(std::string key)
{
	_key = key;
	return (0);
}

int		Channel::set_user_limit(unsigned int limit)
{
	_user_limit = limit;
	return (0);
}

std::string		Channel::get_name(void) const
{
	return (_name);
}

unsigned int	Channel::get_size(void) const
{
	return (_size);
}


/*------------------------------METHODS------------------------------*/

bool	Channel::is_member(Client client) const
{
	if (is_operator(client))
		return (true);

	int	size = _users.size();
	for (int i = 0; i < size; i++)
	{
		if (client == _users[i])
			return (true);
	}
	return (false);
}

bool	Channel::is_operator(Client client) const
{
	int	size = _operators.size();
	for (int i = 0; i < size; i++)
	{
		if (client == _operators[i])
			return (true);
	}
	return (false);
}

bool	Channel::is_invited(Client client) const
{
	int	size = _invited.size();
	for (int i = 0; i < size; i++)
	{
		if (client == _invited[i])
			return (true);
	}
	return (false);
}

int		Channel::add_user(Client new_user)
{
	if (is_member(new_user))
		return (-1);
	if (_limit_set && static_cast<int>(_size) >= _user_limit)
		return (-1);

	_users.push_back(new_user);
	_size++;
	if (is_invited(new_user))
		remove_invite(new_user);
	return (0);
}

int	Channel::remove_member(Client client)
{
	if (is_operator(client))
		_operators.erase(find(_operators.begin(), _operators.end(), client));
	else if (is_member(client))
		_users.erase(find(_users.begin(), _users.end(), client));
	else
		return (-1);
	_size--;
	if (_size == 0)
		return (1);
	return (0);
}

int		Channel::set_to_operator(Client to_operator)
{
	if (!is_member(to_operator))
		return (-1);
	if (is_operator(to_operator))
		return (-1);
	_users.erase(find(_users.begin(), _users.end(), to_operator));
	_operators.push_back(to_operator);
	return (0);
}

int		Channel::set_to_user(Client to_user)
{
	if (!is_member(to_user))
		return (-1);
	if (!is_operator(to_user))
		return (-1);
	_operators.erase(find(_operators.begin(), _operators.end(), to_user));
	_users.push_back(to_user);
	return (0);
}

int		Channel::add_to_invited(Client invited)
{
	if (is_invited(invited))
		return (-1);
	_invited.push_back(invited);
	return (0);
}

int		Channel::remove_invite(Client invited)
{
	if (!is_invited(invited))
		return (-1);
	_invited.erase(find(_invited.begin(), _invited.end(), invited));
	return (0);
}

bool		Channel::check_key(std::string password)
{
	if (password == _key)
		return (true);
	return (false);
}

std::string	Channel::get_members_str()
{
	std::string	s = "";
	int	size = _users.size();
	for (int i = 0; i < size; i++)
		s += _users[i].getNickname() + ' ';
	size = _operators.size();
	for (int i = 0; i < size; i++)
		s += '@' + _operators[i].getNickname() + ' ';
	s.erase(s.size() - 1, 1);
	return (s);
}

