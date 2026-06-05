#ifndef QUIT_HPP
# define QUIT_HPP

# include <iostream>
# include <vector>
# include "Reply.hpp"

class Server;
class Client;

bool	handleQuit(std::vector<std::string> &Token, Server &server, Client &client);

#endif