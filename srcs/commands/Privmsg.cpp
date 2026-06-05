/*  Send private message between users
	Send messages to channels
	ex : 
		PRIVMSG <target> :hello
		PRIVMSG # :hello

		<target> -> is the nickname or the name of a channel
*/

#include "commands/Privmsg.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handlePrivmsg(std::vector<std::string> &Token, Server &server, Client &client) {
	
	std::cout << "HANDLE PRIVATE MESSAGE" << std::endl;
	if (!client.getRegistration())
	{
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "PRIVMSG HANDLER - Client is not registered" << std::endl;
		return (false);
	}
	if (Token.size() == 1)
	{
		server.sendToClient(client.getFd(), ERR_NORECIPIENT());
		std::cerr << "PRIVMSG HANDLER - No recipient" << std::endl;
		return (false);
	}
	if (Token.size() == 2 || Token[2].empty())
	{
		server.sendToClient(client.getFd(), ERR_NOTEXTTOSEND());
		std::cerr << "PRIVMSG HANDLER - No text to send" << std::endl;
		return (false);
	}
	
	Client *target = server.findClient(Token[1]);

	if (target == NULL)
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), Token[1]));
		std::cerr << "PRIVMSG HANDLER - No client with this nickname" << std::endl;
		return (false);
	}
	server.sendToClient(target->getFd(), RPL_TEXTTOSEND(client.getNickname(), 
		client.getUsername(), Token[1], Token[2]));
	std::cout << "INFO: Message sent to user : " << Token[1] << std::endl;
	return (true);
}