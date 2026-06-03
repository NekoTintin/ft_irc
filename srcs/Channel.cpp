#include "Channel.hpp"

// Orthodox Canonical Form
Channel::Channel() : _name(""), _topic("") {}

Channel::~Channel() {}

// Setters
void Channel::setName(const std::string &name) {
	_name = name;
}

void Channel::setTopic(const std::string &topic) {
	_topic = topic;
}

// Getters
const std::string &Channel::getName() const {
	return (_name);
}

const std::string &Channel::getTopic() const {
	return (_topic);
}