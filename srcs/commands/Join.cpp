/*Rejoindre un salon
	ex:
		JOIN #42
		JOIN #42 password
*/

#include "commands/Join.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool tryToJoinChannel(Server &server, Client &client, const std::string &channelName, const std::string &password) {
	// Find if Channel exists
	Channel *channel = server.findChannel(channelName);
	if (channel) {
		// Try to join channel
		if (!channel->addUser(&server, &client, password)) {
			std::cerr << "INFO - USER " << client.getNickname() << " Failed to join channel: " << channelName << std::endl;
			return (false);
		}
	}
	else {
		// Create channel if doesn't exist
		if (!server.createChannel(channelName)) {
			std::cerr << "INFO - USER " << client.getNickname() << " Failed to create channel: " << channelName << std::endl;
			return (false);
		}

		// Join the newly created channel
		channel = server.findChannel(channelName);
		if (!channel->addUser(&server, &client, password)) {
			std::cerr << "INFO - USER " << client.getNickname() << " Failed to join newly created channel: " << channelName << std::endl;
			return (false);
		}
	}
	std::cout << "INFO - USER " << client.getNickname() << " Joined channel: " << channelName << std::endl;
	// Standards JOIN messages
	std::string joinMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost JOIN :" + channelName + "\r\n";
    channel->broadcast(joinMsg, NULL, &server);
	server.sendToClient(client.getFd(), RPL_TOPIC(client.getNickname(), channelName, channel->getTopic()));
	server.sendToClient(client.getFd(), RPL_NAMREPLY(client.getNickname(), "=", channelName, channel->getUsersList()));
	server.sendToClient(client.getFd(), RPL_ENDOFNAMES(client.getNickname(), channelName));
	return (true);
}

bool handleJoin(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing) {
	std::cout << "HANDLE JOIN" << std::endl;

	(void)_hasTrailing;
	// Check Token size
	if (Token.size() < 2) {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
		std::cerr << "JOIN HANDLER - Not enough parameters" << std::endl;
		return (false);
	}

	// Check Registration
	if (!client.getRegistration()) {
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "JOIN HANDLER - Client is not registered" << std::endl;
		return (false);
	}

	std::string	channelsList = Token[1];
	std::string passwordsList = "";
	if (Token.size() > 2)
		passwordsList = Token[2];
	
	// Creates vectors for channels and passwords
	std::vector<std::string> channels;
	std::vector<std::string> passwords;
	std::string item;

	// Split channels and passwords
	std::stringstream ssChannels(channelsList);
	while (std::getline(ssChannels, item, ',')) {
		if (!item.empty())
			channels.push_back(item);
	}
	std::stringstream ssPasswords(passwordsList);
	while (std::getline(ssPasswords, item, ',')) {
		if (!item.empty())
			passwords.push_back(item);
	}

	// Try to join each channel
	for (size_t i = 0; i < channels.size(); ++i) {
		std::string channelName = channels[i];
		std::string password = "";
		if (i < passwords.size())
			password = passwords[i];
		// Basic Checks
		if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&')) {
			server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
			std::cerr << "JOIN HANDLER - Invalid channel name: " << channelName << std::endl;
			continue;
		}
		tryToJoinChannel(server, client, channelName, password);
	}
	return (true);
}