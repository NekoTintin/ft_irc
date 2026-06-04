/*Rejoindre un salon
	ex:
		JOIN #42
		JOIN #42 password
*/

#include "commands/Join.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleJoin(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing)
{
	(void)Token;
	(void)server;
	(void)client;
	(void)_hasTrailing;
	std::cout << "HANDLE JOIN" << std::endl;
	return (true);
}