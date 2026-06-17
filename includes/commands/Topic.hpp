#ifndef TOPIC_HPP
# define TOPIC_HPP

#include <string>
# include <vector>
#include "../Reply.hpp"

class Server;
class Client;

bool	handleTopic(std::vector<std::string> &Token, Server &server, Client &client);

#endif
