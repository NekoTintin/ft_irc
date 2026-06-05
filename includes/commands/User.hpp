#ifndef USER_HPP
# define USER_HPP

#include <iostream>
#include <vector>
#include <string>
#include "Utils.hpp"

class Server;
class Client;

bool	handleUser(std::vector<std::string> &token, Server &server, Client &client);

#endif