/*  Voir ou changer le topic
	TOPIC #42
	TOPIC #42 :Bienvenue    
*/

#include "commands/Topic.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleTopic(std::vector<std::string> &Token, Server &server, Client &client)
{
	(void)Token;
	(void)server;
	(void)client;
	std::cout << "HANDLE TOPIC" << std::endl;
	return (true);
}