#ifndef CAP_HPP
# define CAP_HPP

# include <iostream>
# include <vector>
# include "Reply.hpp"

class Server;
class Client;

bool	handleCap(std::vector<std::string> &Token, Server &server, Client &client);

#endif