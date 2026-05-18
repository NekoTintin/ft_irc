#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

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
		int	_port;
		std::string 			_password;
		int						_socketFD;
		struct sockaddr_in		_serverAddr;
};

# endif