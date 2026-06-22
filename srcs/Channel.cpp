#include "Channel.hpp"

// Orthodox Canonical Form
// Never called
Channel::Channel() :
	_name("#default"),
	_topic(""),
	_isInviteOnly(false),
	_isProtectedTopic(false),
	_channelKey(""),
	_userLimit(0) {}

Channel::Channel(const std::string &name) :
	_name(name),
	_topic(""),
	_isInviteOnly(false),
	_isProtectedTopic(false),
	_channelKey(""),
	_userLimit(0) {}

Channel::~Channel() {}

// Setters
void	Channel::setTopic(const std::string &topic) {
	_topic = topic;
}

void	Channel::setInviteOnly(bool inviteOnly) {
	_isInviteOnly = inviteOnly;
}

void	Channel::setProtectedTopic(bool protectedTopic) {
	_isProtectedTopic = protectedTopic;
}

void	Channel::setChannelKey(const std::string &key) {
	_channelKey = key;
}

void	Channel::setUserLimit(std::size_t limit) {
	_userLimit = limit;
}

// Getters
const	std::string &Channel::getName() const {
	return (_name);
}

const	std::string &Channel::getTopic() const {
	return (_topic);
}

const	std::size_t &Channel::getUserLimit() const {
	return (_userLimit);
}

bool	Channel::isInviteOnly() const {
	return (_isInviteOnly);
}

bool	Channel::isProtectedTopic() const {
	return (_isProtectedTopic);
}

bool	Channel::isUserOnChannel(const Client *client) const {
	return (this->_users.find(client->getFd()) != this->_users.end());
}

bool	Channel::isUserOperator(const Client *client) const {
	return (this->_operators.find(client->getFd()) != this->_operators.end());
}

bool	Channel::isUserInvited(const std::string &nickname) const {
	std::vector<std::string>::const_iterator it;
	for (it = this->_invitedUsers.begin(); it != this->_invitedUsers.end(); ++it) {
		if (*it == nickname)
			return (true);
	}
	return (false);
}

bool	Channel::isEmpty() const {
	return (this->_users.empty());
}

// Funcs
bool	Channel::addUser(Server *server, const Client *client, const std::string &key, bool invited) {
	// Check if user is already on the channel
	if (this->isUserOnChannel(client)) {
		server->sendToClient(client->getFd(), ERR_USERONCHANNEL(client->getNickname(), client->getNickname(), this->_name));
		return (false);
	}
	// Check if channel is full
	// 0 is infinite users
	if (this->_userLimit > 0) {
		if ((this->_users.size() >= this->_userLimit)) {
			server->sendToClient(client->getFd(), ERR_CHANNELISFULL(client->getNickname(), this->_name));
			return (false);
		}
	}
	if (!invited) {
		// Check if channel is invite-only
		if (this->_isInviteOnly) {
			server->sendToClient(client->getFd(), ERR_INVITEONLYCHAN(client->getNickname(), this->_name));
			return (false);
		}
	}
	// Check if channel is key-protected
	if (!this->_channelKey.empty()) {
		if (this->_channelKey != key) {
			server->sendToClient(client->getFd(), ERR_BADCHANNELKEY(client->getNickname(), this->_name));
			return (false);
		}
	}

	// Add user on channel
	this->_users.insert(std::make_pair(client->getFd(), client));
	if (this->_users.size() == 1)
		this->_operators.insert(std::make_pair(client->getFd(), client));

	return (true);
}

bool	Channel::addOperator(const Client *client) {
	if (!this->isUserOnChannel(client))
		return (false);
	this->_operators.insert(std::make_pair(client->getFd(), client));
	return (true);
}

bool	Channel::removeOperator(const Client *client) {
    if (!this->isUserOperator(client))
        return (false);
    this->_operators.erase(client->getFd());
    return (true);
}

bool	Channel::removeUser(const Client *client) {
	// Check if user in on channel
	if (!this->isUserOnChannel(client))
			return (false);
	this->_users.erase(client->getFd());
	this->_operators.erase(client->getFd());
	if (AUTOOP_ON_LEAVE && !this->_users.empty()) {
		if (this->_operators.empty())
			this->_operators.insert(std::make_pair(this->_users.begin()->first, this->_users.begin()->second));
	}
	return (this->_users.empty());
}

// If client is null -> send to everyone
// Otherwise -> send to everyone except client (author)
// msg must be formatted
void	Channel::broadcast(const std::string &msg, const Client *sender, Server *server) {
	std::map<int, const Client*>::const_iterator it;
	for (it = this->_users.begin(); it != this->_users.end(); ++it) {
		if (sender == NULL || it->first != sender->getFd()) {
			server->sendToClient(it->first, msg);
		}
	}
}

std::string	Channel::getUsersList() const {
	std::string	list = "";
	std::map<int, const Client*>::const_iterator it;

	for (it = this->_users.begin(); it != this->_users.end(); ++it) {
		if (it != this->_users.begin())
			list += " ";
		if (this->isUserOperator(it->second))
			list += "@" + it->second->getNickname() + " ";
		else
			list += it->second->getNickname() + " ";
	}
	return (list);
}

std::string	Channel::getModesList() const {
	std::string	list;
	list += "+";
	if (this->_isInviteOnly)
		list += "i";
	if (this->_isProtectedTopic)
		list += "t";
	if (!this->_channelKey.empty())
		list += "k";
	if (this->_userLimit > 0)
		list += "l";

	if (!this->_channelKey.empty())
		list += " " + this->_channelKey;
	if (this->_userLimit > 0) {
		std::ostringstream oss;
		oss << this->_userLimit;
		list += " " + oss.str();
	}
	return (list);
}

bool	Channel::addToInvitedList(const std::string &nickname) {
	if (this->isUserInvited(nickname))
		return (false);
	this->_invitedUsers.push_back(nickname);
	return (true);
}

bool	Channel::removeFromInvitedList(const std::string &nickname) {
	std::vector<std::string>::iterator it;
	for (it = this->_invitedUsers.begin(); it != this->_invitedUsers.end(); ++it) {
		if (*it == nickname) {
			this->_invitedUsers.erase(it);
			return (true);
		}
	}
	return (false);
}