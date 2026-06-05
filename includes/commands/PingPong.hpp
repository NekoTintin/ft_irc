#ifndef PINGPONG_HPP
# define PINGPONG_HPP

# include <iostream>
# include <vector>
# include "Reply.hpp"

class Server;
class Client;

bool	handlePing(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing);
bool	handlePong(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing);

#endif