#ifndef INVITE_HPP
# define INVITE_HPP

# include <iostream>
# include <vector>
# include "Reply.hpp"

class Server;
class Client;

bool	handleInvite(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing);

#endif