#ifndef JOIN_HPP
# define JOIN_HPP

#include <iostream>
# include <vector>
# include "Reply.hpp"

class Server;
class Client;

bool handleJoin(std::vector<std::string> &Token, Server &server, Client &client);

#endif