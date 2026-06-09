/*  Expulser l'utilisateur
	KICK #42 bob
	
*/

#include "commands/Kick.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleKick(std::vector<std::string> &Token, Server &server, Client &client) {
	std::cout << "HANDLE KICK" << std::endl;

	// Check if client is registered
	if (!client.getRegistration()) {
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "KICK HANDLER - Client is not registered" << std::endl;
		return (false);
	}

	// Check if enough parameters
	if (Token.size() < 3 || Token[1].empty() || Token[2].empty()) {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
		std::cerr << "KICK HANDLER - Not enough parameters" << std::endl;
		return (false);
	}

	std::string	channelName = Token[1];
	std::string	targetNickname = Token[2];
	std::string	reason = "No reason specified";
	if (Token.size() > 3) {
		if (Token[3][0] == ':')
			reason = Token[3].substr(1);
		else
			reason = Token[3];
		for (size_t idx = 4; idx < Token.size(); ++idx)
			reason += " " + Token[idx];
	}

	// Check if channel exists
	Channel *channel = server.findChannel(Token[1]);
	if (!channel) {
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), Token[1]));
		std::cerr << "KICK HANDLER - Channel does not exist" << std::endl;
		return (false);
	}

	// Check if client is on the channel
	if (!channel->isUserOnChannel(&client)) {
		server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), Token[1]));
		std::cerr << "KICK HANDLER - Client is not on the channel" << std::endl;
		return (false);
	}

	// Check if client is an operator on the channel
	if (!channel->isUserOperator(&client)) {
		server.sendToClient(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), Token[1]));
		std::cerr << "KICK HANDLER - Client is not an operator on the channel" << std::endl;
		return (false);
	}

	// Check if target user exists
	Client *target = server.findClient(Token[2]);
	if (!target) {
		server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), Token[2]));
		std::cerr << "KICK HANDLER - Target user does not exist" << std::endl;
		return (false);
	}

	// Check if target user is on the channel
	if (!channel->isUserOnChannel(target)) {
		server.sendToClient(client.getFd(), ERR_USERNOTINCHANNEL(client.getNickname(), Token[2], Token[1]));
		std::cerr << "KICK HANDLER - Target user is not on the channel" << std::endl;
		return (false);
	}

	// Kick
	channel->broadcast(RPL_KICK(client.getNickname(), client.getUsername(), channelName, targetNickname, reason), NULL, &server);
	channel->removeUser(target);
	std::cout << "INFO: User " << targetNickname << " has been kicked from channel " << channelName << " by " << client.getNickname() << " because: " << reason << std::endl;
	return (true);
}