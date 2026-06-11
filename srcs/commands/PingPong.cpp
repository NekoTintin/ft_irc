/*  The PING command is sent by either clients or servers to check the other side of the
	connection is still connected and/or to check for connection latency
	Server sends : PING 12345
	Client replies : PONG 12345

	if no answer : Server thinks the client is disonnected
*/

#include "commands/PingPong.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handlePing(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing) {
	std::cout << "HANDLE PING" << std::endl;
	std::string pongStr;
	if ((Token.size() >= 2 && !Token[1].empty()) || _hasTrailing == true) {
		pongStr = ":localhost PONG localhost :" + Token[1];
		server.sendToClient(client.getFd(), pongStr);
		return (true);
	}
	server.sendToClient(client.getFd(), ERR_NOORIGIN());
	return (false);
}

bool handlePong(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing) {
	(void)server;
	std::cout << "HANDLE PONG" << std::endl;
	if ((Token.size() >= 2 && !Token[1].empty()) || _hasTrailing == true) {
		std::cout << "INFO: Received PONG from " << client.getNickname() << std::endl;
		return (true);
	}
	return (true);
}