/*  The QUIT command is used to terminate a client’s connection to the server. 
	The server acknowledges this by replying with an ERROR message and closing the connection to the client.
*/

/*
** QUIT [reason]
**
** Purpose:
** - Terminates the client connection to the server.
**
** Format:
** - QUIT
** - QUIT :<reason>
**
** Rules:
** - The reason is optional.
** - If a reason is provided, it may contain spaces.
** - The server should send an ERROR message before closing
**   the connection.
**
** Current ft_irc implementation:
** - Parse the optional reason.
** - Send an ERROR message to the client.
** - Disconnect the client.
** - Remove the client from the server data structures.
**
** Future improvements (with Channel):
** - Broadcast a QUIT message to users sharing a channel
**   with the leaving client.
** - Remove the client from all joined channels.
*/

#include "commands/Quit.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleQuit(std::vector<std::string> &Token, Server &server, Client &client)
{
	std::cout << "HANDLE QUIT" << std::endl;
	std::string Errormessage = "ERROR :Quit ";
	if (Token.size() >= 2 && !Token[1].empty())
		Errormessage += Token[1];
	else
		Errormessage += "Client Quit";
	server.sendToClient(client.getFd(), Errormessage);
	server.removeClient(client.getFd());
	return (true);
}