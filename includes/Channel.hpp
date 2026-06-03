#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Client.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>

class Channel {
	public:
		Channel();
		~Channel();
		// Setters
		void setName(const std::string &name);
		void setTopic(const std::string &topic);
		// Getters
		const std::string &getName() const;
		const std::string &getTopic() const;
	private:
		// Basics
		std::string _name;
		std::string _topic;
		// Users
		std::map<int, const Client*> _users;
		std::vector<int> _operators;
		// Modes
		bool			_isInviteOnly;
		bool			_isProtectedTopic;
		std::string		channelKey;
		std::size_t		_userLimit;
};

# endif