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

/* Pour tester :
Terminal 1 : valgrind --track-fds=yes ./ircserv 6667 pass
Autres Terminal : nc --crlf 127.0.0.1 6667
Check de la commande : printf "Hello\r\n" | nc 127.0.0.1 6667
Check des commandes incompletes (a faire avec printf pour l'instant):
	{ printf "Hel"; sleep 1; printf "lo\r\n"; } | nc 127.0.0.1 6667
	printf "Hello\r\n123\r\n456" | nc 127.0.0.1 6667
	{ printf "456"; sleep 1; printf "789\r\n"; } | nc 127.0.0.1 6667
	{ printf "NI"; sleep 1; printf "CK malu\r\nUS"; sleep 1; printf "ER malu 0 * :Malu\r\n"; } | nc 127.0.0.1 6667
*/