#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Command.hpp"

int	main(int argc, char **argv) {
	Server server;

	if (!server.checkParsing(argc, argv))
		return (1);
	if (!server.socketSetup())
		return (1);

	server.addServertoPoll();
	server.runServerLoop();

	return (0);
}