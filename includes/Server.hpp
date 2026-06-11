#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <unistd.h>
# include <cstring>
# include <fcntl.h>
# include <vector>
# include <map>
# include <poll.h>
# include <cstdlib>
# include <csignal>
# include <iterator>
# include "Client.hpp"
# include "Command.hpp"
# include "Channel.hpp"
# include "Utils.hpp"
# include "Reply.hpp"

extern bool g_running;

class Server {
	public:
		// Orthodox Canonical Form
		Server();
		Server(int port, const std::string &password);
		~Server();
		// Funcs
		bool		checkParsing(int argc, char **argv);
		bool		socketSetup();
		void		addServertoPoll();
		void		runServerLoop();

		void		removeClient(int fd);
		void		acceptNewClient();
		bool		receiveFromClient(int fd);
		void		cleanServer();

		std::string	getPassword();
		bool		tryRegistration(Client &client);
		bool		NicknameExists(std::string nickname);

		bool		sendToClient(int fd, const std::string &message);

		Client*		findClient(const std::string &Nickname);
		bool	 	ClientExists(int fd);

		// Channels func
		Channel*					findChannel(const std::string &name);
		bool						createChannel(const std::string &name);
		bool						removeChannel(const std::string &name);
		std::vector<std::string>	getChannelList() const;

		// Signal
		static void	signalHandler(int sig);

	private:
		int							_port;
		std::string 				_password;
		int							_socketFD;
		struct sockaddr_in			_serverAddr;

		// List of file descriptors to monitor with poll
		std::vector<pollfd>			_pollfds;

		// List of clients, mapped by their file descriptor
		std::map<int, Client>		_clients;

		// List of channels
		std::map<std::string, Channel> _channels;
};

# endif