#include "commands/Mode.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// Just apply mode, perms are already checked in handleMode
void	applyMode(Client &client, Channel &channel, Server &server, char sign, char mode, const std::string &arg) {
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
			if (sign == '+') {
				if (channel.hasChannelKey()) {
					server.sendToClient(client.getFd(), ERR_KEYSET(client.getNickname(), channel.getName()));
					std::cerr << "MODE HANDLER - Channel key already set" << std::endl;
				}
				else {
					channel.setChannelKey(arg);
				}
			}
			else
				channel.setChannelKey("");
			break;
		case 'o': {
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

	if (Token.size() < 2 || Token[1].empty()) {
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

	// Send channel Operators
	if (Token.size() == 2) {
		server.sendToClient(client.getFd(), RPL_CHANNELMODEIS(client.getNickname(), channel->getName(), channel->getModesList()));
		return (true);
	}

	if (Token[2].empty()) {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		std::cerr << "MODE HANDLER - Missing argument" << std::endl;
		return (false);
	}

	// Check if client is operator
	if (!channel->isUserOperator(&client)) {
		server.sendToClient(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel->getName()));
		std::cerr << "MODE HANDLER - Client is not operator on channel" << std::endl;
		return (false);
	}

	// Parser multimode
	std::string changedModes = "";
	std::string changedArgs = "";

	std::string modes = Token[2];
	char sign = '+';
	size_t idx = 3;
	int		paramsCount = 0;

	for (size_t i = 0; i < modes.size(); ++i) {
		// Iterate on each char
		char c = modes[i];

		if (c == '+' || c == '-') {
			sign = c;
			continue;
		}

		// If mode supported
		if (c != 'i' && c != 't' && c != 'k' && c != 'o' && c != 'l') {
			server.sendToClient(client.getFd(), ERR_UNKNOWNMODE(client.getNickname(), std::string(1, c), channel->getName()));
			std::cerr << "MODE HANDLER - Unknown mode flag: " << c << std::endl;
			continue;
		}

		// If mode requires argument
		bool needsArg = false;
		if (c == 'o')
			needsArg = true;
		else if (c == 'k' && sign == '+')
			needsArg = true;
		else if (c == 'l' && sign == '+')
			needsArg = true;

		std::string arg = "";
		if (needsArg) {
			if (paramsCount >= 3) {
				std::cerr << "MODE HANDLER - Too many arguments for mode: " << sign << c << std::endl;
				continue;
			}
			++paramsCount;
			// Check if argument is provided (below Token.size and not empty)
			if (idx >= Token.size() || Token[idx].empty()) {
				server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
				std::cerr << "MODE HANDLER - Missing argument for mode: " << sign << c << std::endl;
				continue;
			}
			arg = Token[idx];
			++idx;
		}

		// Edges cases
		if (c == 'o') {
			Client *target = server.findClient(arg);
			if (!target) {
				server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), arg));
				std::cerr << "MODE HANDLER - Target client does not exist: " << arg << std::endl;
				continue;
			}
			if (!channel->isUserOnChannel(target)) {
				server.sendToClient(client.getFd(), ERR_USERNOTINCHANNEL(client.getNickname(), target->getNickname(), channel->getName()));
				std::cerr << "MODE HANDLER - Target client is not on channel: " << arg << std::endl;
				continue;
			}
		}
		if (c == 'l' && sign == '+' && std::atoi(arg.c_str()) <= 0) {
			server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
			std::cerr << "MODE HANDLER - Invalid user limit: " << arg << std::endl;
			continue;
		}

		// Apply mode
		applyMode(client, *channel, server, sign, c, arg);

		// Create mode change string
		char lastSign = '\0';
		for (size_t k = changedModes.size(); k > 0; --k) {
			if (changedModes[k - 1] == '+' || changedModes[k - 1] == '-') {
				lastSign = changedModes[k - 1];
				break;
			}
		}
		if (lastSign != sign) {
			changedModes += sign;
		}
		changedModes += c;
		if (needsArg) {
			changedArgs += " " + arg;
		}
	}
	// Send broadcast to channel
	if (!changedModes.empty()) {
		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " " + changedModes + changedArgs;
		channel->broadcast(msg, NULL, &server);
	}
	return (true);
}