#ifndef USER_HPP
# define USER_HPP

#include <vector>
#include <string>
#include "../Reply.hpp"

class Server;
class Client;

// USER cannot be used after registration
bool	handleUser(std::vector<std::string> &token, Server &server, Client &client);

#endif
