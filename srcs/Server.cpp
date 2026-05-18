#include "Server.hpp"

Server::Server(): _port(-1), _password("never") {
	// Never used
}

Server::Server(int port, const std::string &password): _port(port), _password(password) {}

Server::~Server() {}

bool Server::checkParsing(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (false);
	}

	// Port Verification
	char *endptr;
	long port = std::strtol(argv[1], &endptr, 10);
	// Check invalid characters and empty string
	if (endptr == argv[1] || *endptr != '\0') {
		std::cout << "Error: Invalid port number." << std::endl;
		return (false);
	}
	// Check port range
	if (port < 1024 || port > 65535) {
		std::cout << "Error: Port number must be between 1024 and 65535." << std::endl;
		return (false);
	}

	this->_password = argv[2];
	if (this->_password.empty()) {
		std::cout << "Error: Password cannot be empty." << std::endl;
		return (false);
	}
	this->_port = static_cast<int>(port);
	return (true);
}

bool	Server::socketSetup() {
	// AF_INET -> IPv4, SOCK_STREAM --> TCP, 0 --> Default protocol
	this->_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFD < 0) {
		std::cerr << "Error: Failed to create socket." << std::endl;
		return (false);
	}

	// Set socket not blocking
	if (fcntl(this->_socketFD, F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << "Error: Failed to set socket to non-blocking mode." << std::endl;
		close(this->_socketFD);
		return (false);
	}

	// Struct setup + listen all interfaces
	std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_port = htons(this->_port);
	this->_serverAddr.sin_addr.s_addr = INADDR_ANY;

	// Binding
	if (bind(this->_socketFD, (sockaddr*)&this->_serverAddr, sizeof(this->_serverAddr)) < 0) {
		std::cerr << "Error: Failed to bind port " << this->_port << ". (is the port already used ?)" << std::endl;
		close(this->_socketFD);
		return (false);
	}

	// Listen all clients | SOMAXCONN -> 4096
	if (listen(this->_socketFD, SOMAXCONN) < 0) {
		std::cerr << "Error: Failed to listen on port " << this->_port << "." << std::endl;
		close(this->_socketFD);
		return (false);
	}

	std::cout << "Server is currently listening on port '" << this->_port << "'..." << std::endl;
	return (true);
}