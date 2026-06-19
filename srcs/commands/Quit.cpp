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

bool handleQuit(std::vector<std::string> &Token, Server &server, Client &client) {
	std::cout << "HANDLE QUIT" << std::endl;
	
	std::string reason = "Quit: Leaving";
	if (Token.size() > 1)
		reason = "Quit: " + buildTrailingMsg(Token[1]);

	std::vector<std::string> channelList = server.getChannelList();
	std::vector<std::string>::iterator it;
	std::vector<Channel*> joinedChannels;

	// Get all channels where client is here
	for (it = channelList.begin(); it != channelList.end(); ++it) {
		Channel *channel = server.findChannel(*it);
		if (channel->isUserOnChannel(&client))
			joinedChannels.push_back(channel);
	}

	// Broadcast message + remove user from channels
	std::vector<Channel*>::iterator channelIt;
	for (channelIt = joinedChannels.begin(); channelIt != joinedChannels.end(); ++channelIt) {
		(*channelIt)->broadcast(RPL_QUIT(client.getNickname(), client.getUsername(), reason), &client, &server);
		(*channelIt)->removeUser(&client);
		std::cout << "INFO: Client " << client.getNickname() << " removed from channel " << (*channelIt)->getName() << std::endl;

		// Check if channel is empty
		if ((*channelIt)->getUsersList().empty()) {
			std::cout << "INFO: Channel " << (*channelIt)->getName() << " removed (empty)" << std::endl;
			server.removeChannel((*channelIt)->getName());
		}
	}
	server.removeClient(client.getFd());
	return (true);
}