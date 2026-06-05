#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# define AUTOOP_ON_LEAVE 0

# include "Reply.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include <iostream>
# include <string>
# include <map>
# include <vector>

class Channel {
	public:
		// Constructors and Destructor
		Channel();
		Channel(const std::string &name);
		~Channel();

		// Setters
		void	setTopic(const std::string &topic);
		void	setInviteOnly(bool inviteOnly);
		void	setProtectedTopic(bool protectedTopic);
		void	setChannelKey(const std::string &key);
		void	setUserLimit(std::size_t limit);

		// Getters
		const	std::string &getName() const;
		const	std::string &getTopic() const;
		const	std::size_t &getUserLimit() const;
		bool	isInviteOnly() const;
		bool	isProtectedTopic() const;
		bool	isUserOnChannel(const Client *client) const;
		bool	isUserOperator(const Client *client) const;

		// Func
		bool	addUser(Server *server, const Client *client, const std::string &key);
		bool	removeUser(const Client *client);
		void	broadcast(const std::string &msg, const Client *client, Server *server);

	private:
		// Basics
		std::string _name;
		std::string _topic;
		// Users
		std::map<int, const Client*>	_users;
		std::map<int, const Client*>	_operators;
		// Modes
		bool			_isInviteOnly;
		bool			_isProtectedTopic;
		std::string		_channelKey;
		std::size_t		_userLimit;
};

# endif