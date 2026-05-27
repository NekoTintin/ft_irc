#ifndef PASS_HPP
# define PASS_HPP

#include <iostream>
#include <vector>

class Server;
class Client;

bool handlePass(std::vector<std::string> &Token, Server &server, Client &client);

#endif