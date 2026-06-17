#ifndef MODE_HPP
# define MODE_HPP

#include <string>
#include <vector>
#include "../Reply.hpp"

class Server;
class Client;

bool	handleMode(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing);

#endif
