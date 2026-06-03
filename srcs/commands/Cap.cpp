#include "commands/Cap.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleCap(std::vector<std::string> &Token, Server &server, Client &client)
{
	std::cout << "HANDLE CAP" << std::endl;
	for (size_t i = 0; i < Token.size(); i++)
		std::cout << "Token[" << i << "]: " << Token[i] << std::endl;
	if (Token.size() > 1 && Token[1] == "LS")
		server.sendToClient(client.getFd(), ":localhost CAP * LS :\r\n");
	else if (Token.size() > 1 && Token[1] == "END")
		std::cout << "CAP negotiation ended for client " << client.getFd() << std::endl;
	return (true);
}