#include "Server.hpp"

bool g_running = true;

Server::Server(): _port(-1), _password("never") {
	// Never used
}

Server::Server(int port, const std::string &password): _port(port), _password(password) {}

Server::~Server() {}

bool Server::checkParsing(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
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

// Connect the server Socket to the poll() event checker
void	Server::addServertoPoll() {
	pollfd	server_fd;
	server_fd.fd = _socketFD;
	server_fd.events = POLLIN;
	_pollfds.push_back(server_fd);
}

void	Server::removeClient(int fd) {
	if (fd != _socketFD) {
		std::map<int, Client>::iterator it = _clients.find(fd);
		if (it == _clients.end())
			return;
		close(fd);
		for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); ++it) {
			if (it->fd == fd) {
				_pollfds.erase(it);
				break;
			}
		}
		_clients.erase(fd);
		std::cout << "INFO: Client disconnected: fd " << fd << std::endl;
	}
}

void	Server::acceptNewClient() {
	struct sockaddr_in	_clientaddr;
	socklen_t			addr_size = sizeof(_clientaddr);

	int clientFD = accept(_socketFD, (struct sockaddr*)&_clientaddr, &addr_size);

	if (clientFD < 0) {
		std::cerr << "Error: Failed to accept new client." << std::endl;
		return;
	}

	if (fcntl(clientFD, F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << "Error: Failed to set client socket to non-blocking mode." << std::endl;
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
	std::cout << "INFO: New client connected: fd " << clientFD << std::endl;
}

bool	Server::tryRegistration(Client &client) {
	if (client.getCorrectPassword() == false && client.getNicknamestatus() == true && client.getUsernamestatus() == true
			&& client.getRealnamestatus() == true && client.getRegistration() == false) {
		std::cout << "INFO: Client provided wrong password, disconnecting..." << std::endl;
		sendToClient(client.getFd(), ERR_PASSWDMISMATCH(client.getNickname()));
		removeClient(client.getFd());
		return (false);
	}
	if (client.getNicknamestatus() == true && client.getCorrectPassword() == true
				&& client.getUsernamestatus() == true && client.getRealnamestatus() == true
				&& client.getRegistration() == false)
		{
			client.setRegistration();
			std::cout << "INFO: Client registered" << std::endl;
			return (true);
		}
	return (false);
}

bool	Server::receiveFromClient(int fd) {
	char	buffer[1024];
	int		bytesRecv = recv(fd, buffer, sizeof(buffer) - 1, 0);

	// Check for errors and disconnections
	if (bytesRecv < 0) {
		std::cerr << "Error: Failed to receive message from client fd " << fd << "." << std::endl;
		removeClientFromAllChannels(_clients[fd]);
		removeClient(fd);
		return (false);
	}
	// Check for disconnection (recv returns 0)
	else if (bytesRecv == 0) {
		std::cout << "INFO: Connection closed" << std::endl;
		removeClientFromAllChannels(_clients[fd]);
		removeClient(fd);
		return (false);
	}

	buffer[bytesRecv] = '\0';

	std::string data(buffer);
	_clients[fd].addToBuffer(data);
	while (_clients[fd].isComplete()) {
		std::string complete_command = _clients[fd].getCommand();
		_clients[fd].removeCommand();
		if (complete_command.empty())
			continue;
		Command	_newCommand;
		_newCommand.processLine(complete_command, *this, _clients[fd]);
		if (!ClientExists(fd))
			return (false);
		if (tryRegistration(_clients[fd]) == true) {
			sendToClient(fd, RPL_WELCOME(_clients[fd].getNickname(), _clients[fd].getUsername(), "localhost"));
			sendToClient(fd, RPL_YOURHOST(_clients[fd].getNickname()));
			sendToClient(fd, RPL_CREATED(_clients[fd].getNickname(), getDate()));
			sendToClient(fd, RPL_MYINFO(_clients[fd].getNickname()));
		}
	}
	return (true);
}

bool	Server::sendToClient(int fd, const std::string &msg) {
	// Check empty message
	if (msg.empty()) {
		std::cerr << "Error: Cannot send empty message to client fd " << fd << "." << std::endl;
		return (false);
	}

	// Add \r and \n if not present
	std::string formatted_msg = msg;
	if (msg.size() < 2 || msg.compare(msg.size() - 2, 2, "\r\n") != 0)
		formatted_msg += "\r\n";

	// Send to client
	ssize_t bytesSize = send(fd, formatted_msg.c_str(), formatted_msg.size(), MSG_NOSIGNAL);
	if (bytesSize < 0) {
		std::cerr << "Error: Failed to send message to client fd " << fd << "." << std::endl;
		return (false);
	}
	return (true);
}

void	Server::signalHandler(int sig) {
	(void) sig;
	g_running = false;
}

void	Server::cleanServer() {
	for (size_t i = 0; i < _pollfds.size(); i++)
		close(_pollfds[i].fd);

	_pollfds.clear();
	_clients.clear();
}


void	Server::removeClientFromAllChannels(Client &client)
{
	std::vector<std::string> channelsToRemove;

	for (std::map<std::string, Channel>::iterator it = _channels.begin();
		it != _channels.end();++it) {
		Channel &channel = it->second;

		if (channel.isUserOnChannel(&client))
		{
			std::string msg = ":" + client.getNickname() + "!" +
				client.getUsername() + "@localhost QUIT :Client disconnected";

			channel.broadcast(msg, &client, this);

			channel.removeUser(&client);

			if (channel.getUsersList().empty())
				channelsToRemove.push_back(channel.getName());
		}
	}

	for (size_t i = 0; i < channelsToRemove.size(); ++i)
		removeChannel(channelsToRemove[i]);
}
void	Server::runServerLoop() {
	signal(SIGINT, signalHandler);

	while (g_running) {
		int ready = poll(_pollfds.data(), _pollfds.size(), -1);
		if (ready < 0) {
			if (!g_running)
				break;
			std::cerr << "Error: Poll Error" << std::endl;
			break;
		}
		if (ready == 0) {
			std::cerr << "Error: Time out" << std::endl;
			break;
		}

		for (size_t i = 0; i < _pollfds.size(); i++) {
			int fd = _pollfds[i].fd;

			if (fd == _socketFD) {
				if (_pollfds[i].revents & POLLIN)
					acceptNewClient();
				continue;
			}

			if (_pollfds[i].revents & POLLIN) {
				if (!receiveFromClient(fd)) {
					removeClient(fd);
					i--;
					continue;
				}
			}

			if (_pollfds[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
				removeClientFromAllChannels(_clients[fd]);
				removeClient(fd);
				i--;
				continue;
			}
		}
	}
	cleanServer();
}

std::string	Server::getPassword() {
	return (_password);
}

bool Server::NicknameExists(std::string nickname) {
	std::map<int, Client>::iterator it = _clients.begin();

	while (it != _clients.end()) {
		if (nickname == it->second.getNickname())
			return (true);
		it++;
	}
	return (false);
}

Client* Server::findClient(const std::string &Nickname) {
	std::map<int, Client>::iterator it = _clients.begin();

	while (it != _clients.end()) {
		if (Nickname == it->second.getNickname())
			return (&it->second);
		it++;
	}
	return (NULL); 
}

bool Server::ClientExists(int fd) {
	std::map<int, Client>::iterator it = _clients.begin();

	while (it != _clients.end()) {
		if (fd == it->second.getFd())
			return (true);
		it++;
	}
	return (false); 
}

Channel		*Server::findChannel(const std::string &name) {
	std::map<std::string, Channel>::iterator it = _channels.begin();

	while (it != _channels.end()) {
		if (name == it->second.getName())
			return (&it->second);
		it++;
	}
	return (NULL); 
}

bool			Server::createChannel(const std::string &name) {
	if (findChannel(name))
		return (false);
	Channel newChannel(name);
	this->_channels[name] = newChannel;
	std::cout << "INFO: Channel created: " << name << std::endl;
	return (true);
}

bool			Server::removeChannel(const std::string &name) {
	Channel	*toDelete = findChannel(name);
	if (!toDelete)
		return (false);
	std::cout << "INFO: Channel removed: " << name << std::endl;
	this->_channels.erase(name);
	return (true);
}

std::vector<std::string>	Server::getChannelList() const {
	std::vector<std::string> channelNames;
	std::map<std::string, Channel>::const_iterator it = _channels.begin();

	for (it = _channels.begin(); it != _channels.end(); ++it)
		channelNames.push_back(it->second.getName());
	return (channelNames);
}