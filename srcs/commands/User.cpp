/*
    Definit les infos utilisateurs

    nickname = nom affiché dans le chat
    username = 
        - identité technique IRC
        - accepter alphanum, '_' et '-'
    realname = 
        - description utilisateur
        - ok accepte les espaces

    USER <username> 0 * :<realname>
*/

#include "commands/User.hpp"
#include "Server.hpp"
#include "Client.hpp"

bool handleUser(std::vector<std::string> &token, Server &server,
    Client &client)
{
    (void) server;
    std::cout << "HANDLE USER" << std::endl;
    if (token.size() != 5)
    {
        std::cerr << "Wrong number of arguments" << std::endl;
        return (false);
    }
    if (token[2] != "0" || token[3] != "*" ||
            correctname(token[1])==false)
    {
        std::cerr << "Wrong arguments" << std::endl;
        return (false);
    }
    client.setUsername(token[1]);
    client.setRealname(token[4]);
    std::cout << "Username and Realname set" << std::endl;
    return (true);
}

/*

A envoyer dans terminal du client pour tester :

printf "USER username 0 * :realname \r\n" | nc 127.0.0.1 6667

*/