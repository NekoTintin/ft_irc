#ifndef KICK_HPP
# define KICK_HPP

# include <iostream>
# include <vector>
# include "Reply.hpp"

class Server;
class Client;

bool handleKick(std::vector<std::string> &Token, Server &server, Client &client);

#endif