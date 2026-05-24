#include "Server.hpp"

bool g_running = true;

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

	int opt = 1;
	if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
	std::cerr << "Error: setsockopt failed." << std::endl;
	close(_socketFD);
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

//Connect the server Socket to the poll() event checker
void	Server::addServertoPoll()
{
	pollfd	server_fd;
	server_fd.fd = _socketFD;
	server_fd.events = POLLIN;
	_pollfds.push_back(server_fd);
}

void	Server::removeClient(int fd)
{
	close(fd);
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); ++it)
	{
		if (it->fd == fd)
		{
			_pollfds.erase(it);
			break;
		}
	}

	_clients.erase(fd);
}

void	Server::acceptNewClient()
{
	struct sockaddr_in	_clientaddr;
	socklen_t			addr_size = sizeof(_clientaddr);

	int clientFD = accept(_socketFD, (struct sockaddr*)&_clientaddr, &addr_size);

	if (clientFD < 0)
	{
		std::cerr << "New client - Accept error" << std::endl;
		return;
	}

	if (fcntl(clientFD, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "New client - fcntl error" << std::endl;
		close(clientFD);
		return;
	}

	Client	newClient(clientFD);
	_clients[clientFD] = newClient;

	pollfd	newClientFD;
	newClientFD.fd = newClient.getFd();
	newClientFD.events = POLLIN;
	newClientFD.revents = 0;
	_pollfds.push_back(newClientFD);
	std::cout << "New client connected: fd " << clientFD << std::endl;
}

void	Server::receiveFromClient(int fd)
{
	char	buffer[1024];
	int		bytesRecv = recv(fd, buffer, sizeof(buffer) - 1, 0);

	if (bytesRecv < 0)
	{
		std::cerr << "Error with message from client" << std::endl;
		removeClient(fd);
		return;
	}
	else if (bytesRecv == 0)
	{
		std::cout << "Connection closed" << std::endl;
		removeClient(fd);
		return;
	}
		buffer[bytesRecv] = '\0';

		std::string data(buffer);
		_clients[fd].addToBuffer(data);

		std::cout << "Received : " << data <<std::endl;
}

void	Server::signalHandler(int sig)
{
	(void) sig;

	g_running = false;
}

void	Server::cleanServer()
{
	for (size_t i = 0; i < _pollfds.size(); i++)
		close(_pollfds[i].fd);

	_pollfds.clear();
	_clients.clear();
}

void	Server::runServerLoop()
{
	signal(SIGINT, signalHandler);

	while (g_running)
	{
		
		int ready = poll(_pollfds.data(), _pollfds.size(), -1);

		if (ready < 0)
		{
			if (!g_running)
				break;
			std::cerr << "Poll Error" << std::endl;
			break;
		}

		if (ready == 0)
		{
			std::cerr << "Time out" << std::endl;
			break;
		}

		for (size_t i = 0; i < _pollfds.size(); i++)
		{
    		int fd = _pollfds[i].fd;

		    if (_pollfds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
    		{
				if (fd != _socketFD)
				{
					removeClient(fd);
					if (i > 0)
						i--;
				}
				continue;
    		}

			if (fd == _socketFD && (_pollfds[i].revents & POLLIN)) // Socket serveur et connexion entrante
			{
				acceptNewClient();
				continue;
			}

			if (fd != _socketFD && (_pollfds[i].revents & POLLIN))
			{
				receiveFromClient(fd);
				continue;
			}
		}

	}

	cleanServer();
}