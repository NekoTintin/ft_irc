// Permet de donner le mot de passe du serveur

#include "commands/Pass.hpp"
#include "Server.hpp"
#include "Client.hpp"

bool handlePass(std::vector<std::string> &Token, Server &server, Client &client)
{
    std::cout << "HANDLE PASS" << std::endl;
    if (client.getRegistration() == true)
    {
        std::cerr << "Client already registered" << std::endl;
        return (false);
    }
    if (Token.size() != 2 )
    {
        std::cerr << "Wrong number of arguments" << std::endl;
        return (false);
    }
    if (Token[1] != server.getPassword())
    {
        std::cerr << "Incorrect password - Registration denied" << std::endl;
        return (false);
    }

    client.setCorrectPassword();
    std::cout << "Correct Password" << std::endl;
    return (true);
}

/*

A envoyer dans terminal du client pour tester :

printf "PASS pass\r\n" | nc 127.0.0.1 6667


*/