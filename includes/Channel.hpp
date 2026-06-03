#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>

class Channel {
	public:
		Channel();
		~Channel();
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