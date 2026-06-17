#ifndef INVITE_HPP
# define INVITE_HPP

# include <string>
# include <vector>
# include "../Reply.hpp"

class Server;
class Client;
class Channel;

bool	handleInvite(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing);

#endif
