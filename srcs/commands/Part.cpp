// Quitter le channel

#include "commands/Part.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

std::string trailingMsg(std::string &message) {
	std::string result;
	size_t pos = message.find(':');
	if (pos != std::string::npos) {
		result = message.substr(pos + 1);
	}
	return result;
}

bool handlePart(std::vector<std::string> &Token, Server &server, Client &client) {
	std::cout << "HANDLE PART" << std::endl;

	// Check if client is registered
	if (!client.getRegistration()) {
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "PART HANDLER - Client is not registered" << std::endl;
		return (false);
	}

	// Check Token size
	if (Token.size() < 2) {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
		std::cerr << "PART HANDLER - Not enough parameters" << std::endl;
		return (false);
	}

	std::string reason = "Leaving";
	if (Token.size() > 2) {
		if (Token[2][0] == ':')
			reason = Token[2].substr(1);
		else
			reason = Token[2];

		for (size_t idx = 3; idx < Token.size(); ++idx) {
			reason += " " + Token[idx];
		}
	}

	std::vector<std::string> channelsList;
	std::stringstream ss(Token[1]);
	std::string channelName;
	while (std::getline(ss, channelName, ',')) {
		if (!channelName.empty())
			channelsList.push_back(channelName);
	}

	// Check if channel exists
	std::vector<std::string>::iterator it;
	for (it = channelsList.begin(); it != channelsList.end(); ++it) {
		Channel *channel = server.findChannel(*it);
		// Check if client is on the channel
		if (!channel) {
			server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), *it));
			std::cerr << "PART HANDLER - Channel does not exist: " << *it << std::endl;
			continue;
		}
		// If user on channel
		if (channel->isUserOnChannel(&client)) {
			std::string partMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + *it + " :" + reason;
			channel->broadcast(partMsg, NULL, &server);
			channel->removeUser(&client);
			std::cout << "INFO - USER " << client.getNickname() << " Left channel: " << *it << std::endl;
			if (channel->getUsersList().empty()) {
				server.removeChannel(*it);
				std::cout << "INFO - Channel " << *it << " removed (empty)" << std::endl;
			}
		} else {
			server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), *it));
			std::cerr << "PART HANDLER - Client is not on channel: " << *it << std::endl;
			continue;
		}
	}
	return (true);
}