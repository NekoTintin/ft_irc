#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <vector>
#include <map>
#include <poll.h>

#include "Client.hpp"

class Server {
	public:
		// Orthodox Canonical Form
		Server();
		Server(int port, const std::string &password);
		~Server();
		// Funcs
		bool	checkParsing(int argc, char **argv);
		bool	socketSetup();

	private:
		int						_port;
		std::string 			_password;
		int						_socketFD;
		struct sockaddr_in		_serverAddr;

		// Liste des fds surveilles par poll()
		std::vector<pollfd>		_pollfds;
		// Infos de chaque client, accessibles par son fd.
		// Clients[5] contient le Client associe au fd=5.
		std::map<int, Client>	 				
		
};

# endif