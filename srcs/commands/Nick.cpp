// Definit le pseudo du client

#include "commands/Nick.hpp"
#include "Server.hpp"
#include "Client.hpp"

bool nonalphanumchar(char c)
{
    std::string allowedchar;

    allowedchar = "_-[]\\`^{}";
    for (size_t i = 0; i < allowedchar.size(); i++)
    {
        if (allowedchar[i] == c)
            return (true);
    }
    return (false);
}

bool correctNickname(std::string nicknametocheck)
{

    if (!isalpha(nicknametocheck[0]))
        return (false);
    size_t i = 0;
    while (i < nicknametocheck.size())
    {
        if (!isalnum(nicknametocheck[i]) 
            && nonalphanumchar(nicknametocheck[i]) == false)
                return (false);
        i++;
    }
    return (true);
}

bool handleNick(std::vector<std::string> &token, Server &server, Client &client)
{
    std::cout << "HANDLE NICK" << std::endl;
    if (token.size() != 2)
    {
        std::cerr << "Wrong number of arguments" << std::endl;
        return (false);
    }
    if (correctNickname(token[1]) == false)
    {
        std::cerr << "Incorrect Nickname" << std::endl;
        return (false);
    }
    if (server.NicknameExists(token[1]) == true)
    {
        std::cerr << "Nickname is already used by another user" << std::endl;
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