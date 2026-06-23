/*
Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}),
backslashes (\), and pipe (|) characters in nicknames, and MAY disallow digits as the first character.
Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands
*/

#include "commands/Nick.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Utils.hpp"

bool handleNick(std::vector<std::string> &token, Server &server, Client &client, bool _hasTrailing) {
	std::cout << "HANDLE NICK" << std::endl;
	if (token.size() != 2) {
		std::cerr << "NICK HANDLER - Invalid number of arguments" << std::endl;
		server.sendToClient(client.getFd(), ERR_NONICKNAMEGIVEN(client.getNickname()));
		return (false);
	}
	if (correctnamenickname(token[1]) == false || _hasTrailing == true) {
		std::cerr << "NICK HANDLER - Invalid Nickname" << std::endl;
		server.sendToClient(client.getFd(), ERR_ERRONEUSNICKNAME(client.getNickname(), token[1]));
		return (false);
	}
	if (server.NicknameExists(token[1]) == true) {
		std::cerr << "NICK HANDLER - Nickname is already taken by another user" << std::endl;
		server.sendToClient(client.getFd(), ERR_NICKNAMEINUSE(client.getNickname(), token[1]));
		return (false);
	}
	std::string oldNick = client.getNickname();
	client.setNickname(token[1]);
	if (client.getNicknamestatus() && client.getUsernamestatus() && client.getRealnamestatus()) {
		// Broadcast
		std::string nickMsg = ":" + oldNick + "!" + client.getUsername() + "@localhost NICK :" + token[1] + "\r\n";
		server.globalBroadcast(nickMsg, NULL);
	}
	std::cout << "INFO: Nickname set for FD" << client.getFd() << ": " << client.getNickname() << std::endl;
	return (true);
}
