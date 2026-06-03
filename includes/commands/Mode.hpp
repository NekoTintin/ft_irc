#ifndef MODE_HPP
# define MODE_HPP

#include <iostream>
#include <vector>
#include "Reply.hpp"

class Server;
class Client;

bool handleMode(std::vector<std::string> &Token, Server &server, Client &client);

#endif