//Quitter le channel

#include "commands/Part.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handlePart(std::vector<std::string> &Token, Server &server, Client &client)
{
	(void)Token;
	(void)server;
	(void)client;
	std::cout << "HANDLE PART" << std::endl;
	return (true);
}
