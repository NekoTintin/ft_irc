/*  Deconnexion propre
	QUIT :bye
*/

#include "commands/Quit.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleQuit(std::vector<std::string> &Token, Server &server, Client &client)
{
	(void)Token;
	(void)server;
	(void)client;
	std::cout << "HANDLE QUIT" << std::endl;
	return (true);
}