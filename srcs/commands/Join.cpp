/*Rejoindre un salon
	ex:
		JOIN #42
		JOIN #42 password
*/

#include "commands/Join.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleJoin(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing) {
	(void)Token;
	(void)_hasTrailing;
	std::cout << "HANDLE JOIN" << std::endl;
	if (!client.getRegistration())
	{
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "JOIN HANDLER - Client is not registered" << std::endl;
		return (false);
	}
	return (true);
}