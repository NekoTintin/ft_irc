#include "commands/Cap.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleCap(std::vector<std::string> &Token, Server &server, Client &client) {
	std::cout << "HANDLE CAP" << std::endl;
	// Start CAP negotiation
	if (Token.size() > 1 && Token[1] == "LS")
		server.sendToClient(client.getFd(), ":localhost CAP * LS :\r\n");
	// End CAP negotiation
	else if (Token.size() > 1 && Token[1] == "END")
		std::cout << "INFO: CAP negotiation ended for client " << client.getFd() << std::endl;
	return (true);
}