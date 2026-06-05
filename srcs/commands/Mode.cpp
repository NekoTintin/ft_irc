/*  5 modes disponibles
i : set/remove invite only
t : seul operateur peut changer le topic
k : set/remove mdp channel
o : give/take channel operator privilege
l : set/remove limite utilisateurs

	MODE #42 +k motdepasse
	MODE #42 +o marie
	MODE #42 +l 10
	MODE #42 -i
*/

#include "commands/Mode.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleMode(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing)
{
	(void)Token;
	(void) _hasTrailing;
	std::cout << "HANDLE MODE" << std::endl;
	if (!client.getRegistration())
	{
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "Wrong - Client is not registered" << std::endl;
		return (false);
	}
	return (true);
}