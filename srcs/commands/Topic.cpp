/*  Voir ou changer le topic
	TOPIC #42
	TOPIC #42 :Bienvenue
*/

#include "commands/Topic.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// Args -> a[0] = TOPIC | a[1] = #42 | a[2] = Bienvenue
// If a[2] is empty -> show topic
bool handleTopic(std::vector<std::string> &Token, Server &server, Client &client) {
	std::cout << "HANDLE TOPIC" << std::endl;
	//Client not registered
	if (!client.getRegistration()) {
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "TOPIC HANDLER - Client is not registered" << std::endl;
		return (false);
	}

	// Not enough args
	if (Token.size() < 2 || Token[1].empty()) {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), Token[1]));
		std::cerr << "TOPIC HANDLER - Not enough parameters" << std::endl;
		return (false);
	}

	// Check if channel exist
	Channel *toExist = server.findChannel(Token[1]);
	if (!toExist) {
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), Token[1]));
		std::cerr << "TOPIC HANDLER - No channel with this name" << std::endl;
		return (false);
	}

	// Is user in Channel
	if (!toExist->isUserOnChannel(&client)) {
		server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), Token[1]));
		std::cerr << "TOPIC HANDLER - Client is not on the channel" << std::endl;
		return (false);
	}

	// Only Read topic
	if (Token.size() == 2) {
		if (toExist->getTopic().empty()) {
			server.sendToClient(client.getFd(), RPL_NOTOPIC(client.getNickname(), toExist->getName()));
		} else {
			server.sendToClient(client.getFd(), RPL_TOPIC(client.getNickname(), toExist->getName(), toExist->getTopic()));
		}
		return (true);
	}

	// Is Topic user locked ?
	if (toExist->isProtectedTopic()) {
		if (!toExist->isUserOperator(&client)) {
			server.sendToClient(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), Token[1]));
			std::cerr << "TOPIC HANDLER - Client is not an operator on the channel" << std::endl;
			return (false);
		}
	}

	std::string newTopic = "";
	if (Token[2][0] == ':') {
		newTopic = Token[2].substr(1);
	} else {
		newTopic = Token[2];
	}
	for (size_t idx = 3; idx < Token.size(); ++idx) {
		newTopic += " " + Token[idx];
	}
	toExist->setTopic(newTopic);
	// Broadcast to everyone
	std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost TOPIC " + toExist->getName() + " :" + toExist->getTopic();
	toExist->broadcast(msg, NULL, &server);
	std::cout << "INFO: Topic changed for channel " << toExist->getName() << std::endl;
	return (true);
}