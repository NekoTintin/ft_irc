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

// Just apply mode, perms are already checked in handleMode
void	applyMode(std::vector<std::string> &Token, Client &client, Channel &channel) {
	char mode = Token[2][0];
	std::string arg = "";
	if (Token.size() > 3)
		arg = Token[3];

	switch (mode) {
		case 'i':
			std::cout << "HANDLE MODE: " << client.getNickname() << " is setting invite only mode on channel " << channel.getName() << std::endl;
			channel.setInviteOnly(true);
			break;
		case 't':
			std::cout << "HANDLE MODE: " << client.getNickname() << " is setting topic protection mode on channel " << channel.getName() << std::endl;
			channel.setProtectedTopic(true);
			break;
		case 'k':
			std::cout << "HANDLE MODE: " << client.getNickname() << " is setting password mode on channel " << channel.getName() << std::endl;
			channel.setChannelKey(arg);
			break;
		case 'o':
			std::cout << "HANDLE MODE: " << client.getNickname() << " made operator for " << channel.getName() << std::endl;
			channel.addOperator(&client);
			break;
		case 'l':
			std::cout << "HANDLE MODE: " << client.getNickname() << " is setting user limit to " << arg << " on channel " << channel.getName() << std::endl;
			channel.setUserLimit(std::atoi(arg.c_str()));
			break;
		default:
			std::cerr << "Unknown mode: " << mode << " (will be ignored)" << std::endl;
	}
}

bool	handleMode(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing) {
	(void)Token;
	(void) _hasTrailing;
	std::cout << "HANDLE MODE" << std::endl;

	// Is client registered ?
	if (!client.getRegistration()) {
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "MODE HANDLER - Client is not registered" << std::endl;
		return (false);
	}
	
	return (true);
}