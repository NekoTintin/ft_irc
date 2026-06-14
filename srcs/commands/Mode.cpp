#include "commands/Mode.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// Just apply mode, perms are already checked in handleMode
void	applyMode(std::vector<std::string> &Token, Client &client, Channel &channel, Server &server) {

	char sign = Token[2][0];
	char mode = Token[2][1];
	std::string arg = "";
	if (Token.size() > 3)
		arg = Token[3];

	switch (mode) {
		case 'i':
			std::cout << "HANDLE MODE: " << client.getNickname() << " is setting invite only mode on channel " << channel.getName() << std::endl;
			if (sign == '+')
				channel.setInviteOnly(true);
			else
				channel.setInviteOnly(false);
			break;
		case 't':
			std::cout << "HANDLE MODE: " << client.getNickname() << " is setting topic protection mode on channel " << channel.getName() << std::endl;
			if (sign == '+')
				channel.setProtectedTopic(true);
			else
				channel.setProtectedTopic(false);
			break;
		case 'k':
			std::cout << "HANDLE MODE: " << client.getNickname() << " is setting password mode on channel " << channel.getName() << std::endl;
			if (sign == '+')
				channel.setChannelKey(arg);
			else
				channel.setChannelKey("");
			break;
		case 'o':
		{
			Client *target = server.findClient(arg);
			std::cout << "HANDLE MODE: " << client.getNickname() << " made operator for " << channel.getName() << std::endl;
			if (sign == '+')
				channel.addOperator(target);
			else
				channel.removeOperator(target);
			break;
		}
		case 'l':
				std::cout << "HANDLE MODE: " << client.getNickname() << " is setting user limit to " << arg << " on channel " << channel.getName() << std::endl;
				if (sign == '+')
					channel.setUserLimit(std::atoi(arg.c_str()));
				else
					channel.setUserLimit(0);
			break;
		default:
			std::cerr << "Unknown mode: " << mode << " (will be ignored)" << std::endl;
	}
}

bool	handleMode(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing) {

	(void) _hasTrailing;
	std::cout << "HANDLE MODE" << std::endl;
	// Is client registered ?
	if (!client.getRegistration()) {
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "MODE HANDLER - Client is not registered" << std::endl;
		return (false);
	}
	//  All are arguments ok ?
	if (Token.size() < 3 || Token[2].empty() || Token[2].size() < 2 ) {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		std::cerr << "MODE HANDLER - Missing argument" << std::endl;
		return (false);
	}
	// Channel exists ?
	Channel *channel = server.findChannel(Token[1]);
	if (!channel) {
    	server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), Token[1]));
    	std::cerr << "MODE HANDLER - Channel does not exist" << std::endl;
		return (false);
	}
	// Is Client in the Channel ?
	if (!channel->isUserOnChannel(&client)) {
		server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), channel->getName()));
		std::cerr << "MODE HANDLER - Client is not on channel" << std::endl;
		return (false);
	}
	// Is Client an Operator ?
	if (!channel->isUserOperator(&client)) {
		server.sendToClient(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel->getName()));
		std::cerr << "MODE HANDLER - Client in not an operator" << std::endl;
		return (false);
	}
	// Is there a sign before the mode
	if (Token[2][0] != '+' && Token[2][0] != '-') {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		std::cerr << "MODE HANDLER - Missing sign for mode" << std::endl;
		return (false);
	}

	char sign = Token[2][0];
	char mode = Token[2][1];
	std::string arg = "";
	if (Token.size() > 3)
		arg = Token[3];
	// Error if combined mode ?
	if (Token[2].size() > 2 || (mode != 'i' && mode != 't' && mode != 'k' && mode != 'o'
		&& mode != 'l')) {
		server.sendToClient(client.getFd(), ERR_UNKNOWNMODE(Token[2]));
		std::cerr << "MODE HANDLER - Combined mode not supported" << std::endl;
		return (false);
	}
	if (mode == 'o')
	{
		if (Token.size() != 4)
		{
			server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
    		std::cerr << "MODE HANDLER - Wrong number of args" << std::endl;
			return (false);
		}
		Client *target = server.findClient(arg);
		if (target == NULL)
		{
			server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), arg));
			std::cerr << "MODE HANDLER - Target not found" << std::endl;
			return (false);
		}
		if (!channel->isUserOnChannel(target)) {
			server.sendToClient(client.getFd(), ERR_USERNOTINCHANNEL(client.getNickname(), target->getNickname(), channel->getName()));
			std::cerr << "MODE HANDLER - User not in channel" << std::endl;
			return (false);
		}
	}
	if (mode == 'l' && sign == '+' && Token.size() != 4)
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		std::cerr << "MODE HANDLER - Wrong number of args" << std::endl;
		return (false);
	}
	else if (mode == 'l' && sign == '+' && atoi(Token[3].c_str()) <= 0)
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		std::cerr << "MODE HANDLER - Incorrect limit" << std::endl;
		return (false);
	}
	if (mode == 'l' && sign == '-' && Token.size() != 3)
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		std::cerr << "MODE HANDLER - Wrong number of args" << std::endl;
		return (false);	
	}
	if ((mode == 'i' || mode == 't') && Token.size() != 3)
    {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		std::cerr << "MODE HANDLER - Wrong number of args" << std::endl;
		return (false);
	}
	if ((mode == 'k') && sign == '+' && Token.size() != 4)
   	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		std::cerr << "MODE HANDLER - Wrong number of args" << std::endl;
		return (false);
	}
	if ((mode == 'k') && sign == '-' && Token.size() != 3)
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		std::cerr << "MODE HANDLER - Wrong number of args" << std::endl;
		return (false);
	}
	applyMode(Token, client, *channel, server);
	//broadcast
	std::string msg = ":" + client.getNickname() + "!" + client.getUsername() +
		"@localhost MODE " + channel->getName() + " " + Token[2];
	if (Token.size() > 3)
		msg += " " + Token[3];
	channel->broadcast(msg, NULL, &server);
	return (true);
}