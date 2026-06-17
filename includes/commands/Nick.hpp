#ifndef NICK_HPP
# define NICK_HPP

# include <string>
# include <vector>
# include <cctype>
# include <string>

class Server;
class Client;

bool	handleNick(std::vector<std::string> &token, Server &server, Client &client, bool _hasTrailing);
#endif
