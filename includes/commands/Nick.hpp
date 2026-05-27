#ifndef NICK_HPP
# define NICK_HPP

#include <iostream>
#include <vector>
#include <cctype>
#include <string>

class Server;
class Client;

bool nonalphanumchar(char c);
bool correctNickname(std::string nicknametocheck);
bool handleNick(std::vector<std::string> &token, Server &server,
        Client &client);
#endif