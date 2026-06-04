/*
Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}),
backslashes (\), and pipe (|) characters in nicknames, and MAY disallow digits as the first character.
Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands
*/

#include "commands/Nick.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Utils.hpp"

bool handleNick(std::vector<std::string> &token, Server &server, Client &client, bool _hasTrailing)
{
	std::cout << "HANDLE NICK" << std::endl;
	if (token.size() != 2)
	{
		std::cerr << "Wrong number of arguments" << std::endl;
		server.sendToClient(client.getFd(), ERR_NONICKNAMEGIVEN(client.getNickname()));
		return (false);
	}
	if (correctnamenickname(token[1]) == false || _hasTrailing == true)
	{
		std::cerr << "Incorrect Nickname" << std::endl;
		server.sendToClient(client.getFd(), ERR_ERRONEUSNICKNAME(client.getNickname(), token[1]));
		return (false);
	}
	if (server.NicknameExists(token[1]) == true)
	{
		std::cerr << "Nickname is already used by another user" << std::endl;
		server.sendToClient(client.getFd(), ERR_NICKNAMEINUSE(client.getNickname(), token[1]));
		return (false);
	}
	client.setNickname(token[1]);
	std::cout << "Nickname set" << std::endl;
	return (true);
}
