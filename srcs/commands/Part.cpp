//Quitter le channel

#include "commands/Part.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handlePart(std::vector<std::string> &Token, Server &server, Client &client)
{
	(void)Token;
	std::cout << "HANDLE PART" << std::endl;
	if (!client.getRegistration())
	{
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "PART HANDLER - Client is not registered" << std::endl;
		return (false);
	}
	return (true);
}
