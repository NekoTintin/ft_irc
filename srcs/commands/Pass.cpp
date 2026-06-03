// Permet de donner le mot de passe du serveur

#include "commands/Pass.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handlePass(std::vector<std::string> &Token, Server &server, Client &client)
{
	std::cout << "HANDLE PASS" << std::endl;
	if (client.getRegistration() == true)
	{
		server.sendToClient(client.getFd(), ERR_ALREADYREGISTRED(client.getNickname()));
		return (false);
	}
	if (Token.size() != 2)
	{
		std::string username = client.getNickname().empty() ? "*" : client.getNickname();
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(username, "PASS"));
		return (false);
	}
	if (Token[1] != server.getPassword())
	{
		//server.sendToClient(client.getFd(), formatMessage(ERR_PASSWDMISMATCH, client, channel));
		return (false);
	}

	client.setCorrectPassword();
	std::cout << "Correct Password" << std::endl;
	return (true);
}

/*
A envoyer dans terminal du client pour tester :

nc --crlf 127.0.0.1 6667

printf "PASS pass\r\n" | nc 127.0.0.1 6667
*/