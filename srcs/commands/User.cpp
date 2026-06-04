/*
	- Defines the username and realname of a client.
	- Used during the registration phase.

	USER <username> 0 * :<realname>

	Rules:
	- username must not be empty.
	- realname must be the last parameter.
	- realname may contain spaces and is prefixed by ':'.
	- the second and third parameters should be "0" and "*".
	- username and realname do not need to be unique.

	Errors:
	- ERR_NEEDMOREPARAMS (461):
	 missing or invalid parameters.
	
	 - ERR_ALREADYREGISTERED (462):
	USER sent after the client has already completed registration.

	Registration:
	 - USER alone does not register the client.
	
	 - Registration is completed only when:
		PASS valid
		NICK set
		USER set
*/

#include "commands/User.hpp"
#include "Server.hpp"
#include "Client.hpp"

bool handleUser(std::vector<std::string> &token, Server &server, Client &client) {
	(void)server;
	std::cout << "HANDLE USER" << std::endl;
	if (client.getRegistration())
	{
		server.sendToClient(client.getFd(), ERR_ALREADYREGISTRED(client.getNickname()));
		std::cerr << "Wrong - already registered" << std::endl;
		return (false);		
	}
	if (token.size() != 5 || token[2] != "0" || token[3] != "*" || token[1].empty() || token[4].empty()
		|| correctname(token[1]) == false) {
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "USER"));
		std::cerr << "Wrong arguments" << std::endl;
		return (false);
	}
	client.setUsername(token[1]);
	client.setRealname(token[4]);
	std::cout << "Username and Realname set" << std::endl;
	return (true);
}