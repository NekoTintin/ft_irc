/*  Inviter un utilisateur
	INVITE bob #42
	Statut d'Operateur est obligatoire sur le salon ou on invite
	un autre utilisateur
*/

#include "commands/Invite.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleInvite(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing) {
	std::cout << "HANDLE INVITE" << std::endl;
	(void)_hasTrailing;
	
	// Check if client is registered
	if (!client.getRegistration()) {
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "INVITE HANDLER - Client is not registered" << std::endl;
		return (false);
	}

	// Verify arguments
	if (Token.size() < 3) {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
		std::cerr << "INVITE HANDLER - Not enough parameters" << std::endl;
		return (false);
	}

	// Is target client valid ?
	Client *targetClient = server.findClient(Token[1]);
	if (!targetClient) {
		server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), Token[1]));
		std::cerr << "INVITE HANDLER - Target client does not exist" << std::endl;
		return (false);
	}

	// Is channel valid ?
	Channel *channel = server.findChannel(Token[2]);
	if (!channel) {
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), Token[2]));
		std::cerr << "INVITE HANDLER - Channel does not exist" << std::endl;
		return (false);
	}

	// Is client on channel ?
	if (!channel->isUserOnChannel(&client)) {
		server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), Token[2]));
		std::cerr << "INVITE HANDLER - Client is not on channel" << std::endl;
		return (false);
	}

	// Is client an operator on channel ?
	if (channel->isInviteOnly() && !channel->isUserOperator(&client)) {
		server.sendToClient(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(),Token[2]));
		std::cerr << "INVITE HANDLER - Client is not an operator on channel" << std::endl;
		return (false);
	}

	// Check if target client is already on the channel
	if (channel->isUserOnChannel(targetClient)) {
		server.sendToClient(client.getFd(), ERR_USERONCHANNEL(client.getNickname(), targetClient->getNickname(), channel->getName()));
		std::cerr << "INVITE HANDLER - Target client is already on channel" << std::endl;
		return (false);
	}

	// Add to invited list
	channel->addToInvitedList(targetClient->getNickname());
	server.sendToClient(client.getFd(), 
		RPL_INVITING(client.getNickname(), targetClient->getNickname(), channel->getName()));
	std::string msg = INVITE_MSG(client.getNickname(), client.getUsername(), targetClient->getNickname(), channel->getName());
	server.sendToClient(targetClient->getFd(), msg);
	return (true);
}