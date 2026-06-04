/*  Voir ou changer le topic
	TOPIC #42
	TOPIC #42 :Bienvenue
*/

#include "commands/Topic.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// Args -> a[0] = TOPIC | a[1] = #42 | a[2] = Bienvenue
bool handleTopic(std::vector<std::string> &Token, Server &server, Client &client)
{
	std::cout << "HANDLE TOPIC" << std::endl;
	// Not enough args
	if (Token.size() < 2) {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), Token[1]));
		return (false);
	}
	// Check if channel exist
	Channel *toExist = server.findChannel(Token[1]);
	if (!toExist) {
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), Token[1]));
		return (false);
	}
	// Is user in Channel
	if (!toExist->isUserOnChannel(&client)) {
		server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), Token[1]));
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
			return (false);
		}
	}
	toExist->setTopic(Token[3]);
	// Broadcast to everyone
	std::string msg = ":" + client.getNickname() + " TOPIC " + toExist->getName() + " :" + toExist->getTopic();
	toExist->broadcast(msg, NULL, &server);
	return (true);
}