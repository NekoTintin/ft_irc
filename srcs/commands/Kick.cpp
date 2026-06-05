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
	std::cout << "HANDLE KICK" << std::endl;
	if (!client.getRegistration())
	{
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "Wrong - Client is not registered" << std::endl;
		return (false);
	}
	return (true);
}