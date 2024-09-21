#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "includes.hpp"
# include "Client.hpp"
# include "Server.hpp"

class Client;
class Server;

class Channel
{

 	private:
	
		std::string 		_name;
		std::string 		_key; //password
		std::string 		_topic;
		int					_user_limit;

		unsigned int		_size;

		bool				_invite_only;
		bool				_topic_restrict;
		bool				_key_set;
		bool				_limit_set;

		std::vector<Client> _users;
		std::vector<Client> _operators;
		std::vector<Client> _invited;

	public:

		/* CONSTRUCTOR DESTRUCTOR */
		Channel(Client channel_creator, std::string name, std::string key = "");
		Channel(Channel const &src);
		~Channel();

		/* OPERATOR OVERLOAD*/
		Channel	&operator=(Channel const &rhs);

		/* ACCESSORS */
		std::vector<Client>	get_operators()	const;
		std::vector<Client>	get_users()		const;
		std::vector<Client>	get_invited()	const;

		bool	is_invite_only(void) const;
		bool	is_topic_restrict(void) const;
		bool	is_key_set(void) const;
		bool	is_limit_set(void) const;

		int		toggle_invite_only(void);
		int		toggle_topic_restrict(void);
		int		toggle_key_set(void);
		int		toggle_limit_set(void);

		int				get_user_limit(void) const;
		std::string		get_topic(void) const;
		std::string		get_key(void) const;

		int		set_topic(std::string topic);
		int		set_key(std::string key);
		int		set_user_limit(unsigned int limit);

		std::string		get_name(void) const;
		unsigned int	get_size(void) const;


		/* METHODS */
		bool	is_member(Client client) const;
		bool	is_operator(Client client) const;
		bool	is_invited(Client client) const;

		int		add_user(Client new_user);
		int		remove_member(Client to_remove);
		int		set_to_operator(Client to_operator);
		int		set_to_user(Client to_user);
		int		add_to_invited(Client invited);
		int		remove_invite(Client invited);

		bool	check_key(std::string password);

		std::string	get_members_str(void);
		
 
};

#endif