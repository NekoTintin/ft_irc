#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <string>

class Channel {
	public:
		Channel();
		~Channel();
	private:
		std::string _name;
		std::string _topic;
}

# endif