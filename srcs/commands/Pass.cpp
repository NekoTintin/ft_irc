#include "commands/Pass.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handlePass(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing) {
	std::cout << "HANDLE PASS" << std::endl;
	if (client.getRegistration() == true) {
		server.sendToClient(client.getFd(), ERR_ALREADYREGISTRED(client.getNickname()));
		return (false);
	}

	if (Token.size() != 2 || _hasTrailing == true) {
		std::string username;
		if (client.getNickname().empty())
			username = "*";
		else
			username = client.getNickname();
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(username, "PASS"));
		return (false);
	}

	if (Token[1] != server.getPassword()) {
		server.sendToClient(client.getFd(), ERR_PASSWDMISMATCH(client.getNickname()));
		return (false);
	}
	client.setCorrectPassword();
	std::cout << "INFO: Correct Password for client " << client.getFd() << std::endl;
	return (true);
}