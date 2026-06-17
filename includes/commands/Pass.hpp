#ifndef PASS_HPP
# define PASS_HPP

# include <string>
# include <vector>
# include "../Reply.hpp"

class Server;
class Client;
class Channel;

bool	handlePass(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing);

#endif
