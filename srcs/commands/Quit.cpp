/*  The QUIT command is used to terminate a client’s connection to the server. 
	The server acknowledges this by replying with an ERROR message and closing the connection to the client.
*/

#include "commands/Quit.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleQuit(std::vector<std::string> &Token, Server &server, Client &client)
{
	(void)Token;
	(void)server;
	(void)client;
	std::cout << "HANDLE QUIT" << std::endl;
	return (true);
}