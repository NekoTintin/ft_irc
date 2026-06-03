/*  Expulser l'utilisateur
	KICK #42 bob
	
*/

#include "commands/Kick.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleKick(std::vector<std::string> &Token, Server &server, Client &client)
{
	(void)Token;
	(void)server;
	(void)client;
	std::cout << "HANDLE KICK" << std::endl;
	return (true);
}