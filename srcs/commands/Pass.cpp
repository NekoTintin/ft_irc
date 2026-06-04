#include "commands/Pass.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handlePass(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing)
{
	std::cout << "HANDLE PASS" << std::endl;
	if (client.getRegistration() == true)
	{
		server.sendToClient(client.getFd(), ERR_ALREADYREGISTRED(client.getNickname()));
		return (false);
	}
	if (Token.size() != 2 || _hasTrailing == true)
	{
		std::string username = client.getNickname().empty() ? "*" : client.getNickname();
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(username, "PASS"));
		return (false);
	}
	if (Token[1] != server.getPassword())
		return (false);
	client.setCorrectPassword();
	std::cout << "Correct Password" << std::endl;
	return (true);
}