#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

# include <iostream>
# include <vector>
# include "Reply.hpp"

class Server;
class Client;


bool handlePrivmsg(std::vector<std::string> &Token, Server &server, Client &client);

#endif
