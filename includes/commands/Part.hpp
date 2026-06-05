#ifndef PART_HPP
# define PART_HPP

# include <iostream>
# include <vector>
# include "Reply.hpp"

class Server;
class Client;

bool	handlePart(std::vector<std::string> &Token, Server &server, Client &client);

#endif