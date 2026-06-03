/*  Voir ou changer le topic
	TOPIC #42
	TOPIC #42 :Bienvenue
*/

#include "commands/Topic.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// Args -> a[0] = TOPIC | a[1] = #42 | a[2] = Bienvenue
bool handleTopic(std::vector<std::string> &Token, Server &server, Client &client, Channel &channel)
{
	std::cout << "HANDLE TOPIC" << std::endl;
	// Not enough args
	if (Token.size() < 2) {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), Token[1]));
		return (false);
	}
	// Check if channel exist
	// TODO no channel list on server
	// Is user in Channel
	if (!channel.isUserOnChannel(client)) {
		server.sendToClient(client->getFd(), ERR_NOTONCHANNEL(client.getNickname(), Token[1]));
		return (false);
	}
	// Only Read topic
	if (Token.size() == 2) {
		if (channel.getTopic().empty()) {
			server.sendToClient(client.getFd(), RPL_NOTOPIC(client.getNickname(), channel.getName()));
		} else {
			server.sendToClient(client.getFd(), RPL_TOPIC(client.getNickname(), channel.getName(), channel.getTopic()));
		}
		return (true);
	}
	// Is Topic user locked ?
	if (channel.isProtectedTopic())
		if (!channel.isUserOperator(client)) {
			server.sendToClient(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), Token[1]));
			return (false);
		}
	}
	channel.setTopic(Token[3])
	// Broadcast to everyone
	std::string msg = ":" + client.getNickname() + " TOPIC " + channel.getName() + " :" + channel.getTopic();
	channel.broadcast(msg, NULL, &server);
	return (true);
}