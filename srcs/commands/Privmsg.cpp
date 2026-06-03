/* Envoyer un message prive ou dans un channel
	ex : 
		PRIVMSG utilisateur :hello
		PRIVMSG # :hello
*/

#include "commands/Privmsg.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handlePrivmsg(std::vector<std::string> &Token, Server &server, Client &client)
{
	(void)Token;
	(void)server;
	(void)client;
	std::cout << "HANDLE PRIVATE MESSAGE" << std::endl;
	return (true);
}