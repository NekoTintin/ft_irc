// Definit le pseudo du client

#include "commands/Nick.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Utils.hpp"

// CHECKER SI TOKEN[1] EST VIDE 

bool handleNick(std::vector<std::string> &token, Server &server, Client &client)
{
    std::cout << "HANDLE NICK" << std::endl;
    if (token.size() != 2)
    {
        std::cerr << "Wrong number of arguments" << std::endl;
        server.sendToClient(client.getFd(), ERR_NONICKNAMEGIVEN(client.getNickname()));
        return (false);
    }
    if (correctnamenickname(token[1]) == false)
    {
        std::cerr << "Incorrect Nickname" << std::endl;
        server.sendToClient(client.getFd(), ERR_ERRONEUSNICKNAME(client.getNickname(), token[1]));
        return (false);
    }
    if (server.NicknameExists(token[1]) == true)
    {
        std::cerr << "Nickname is already used by another user" << std::endl;
        server.sendToClient(client.getFd(), ERR_NICKNAMEINUSE(client.getNickname(), token[1]));
        return (false);
    }
    client.setNickname(token[1]);
    std::cout << "Nickname set" << std::endl;
    return (true);
}

/*

A envoyer dans terminal du client pour tester :

printf "NICK nickiki\r\n" | nc 127.0.0.1 6667

*/