#ifndef NICK_HPP
# define NICK_HPP

#include <iostream>
#include <vector>
#include <cctype>
#include <string>

class Server;
class Client;

bool handleNick(std::vector<std::string> &token, Server &server,
        Client &client);
#endif