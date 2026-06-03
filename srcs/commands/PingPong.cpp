/*  Verifier qu'une connexion est toujours vivante
    Serveur envoie : PING 12345
    Client repond : PONG 12345

    Si pas de reponse, le serveur considere que le client est mort/deconnecte
*/

#include "commands/PingPong.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handlePing(std::vector<std::string> &Token, Server &server, Client &client)
{
    (void)Token;
    (void)server;
    (void)client;
    std::cout << "HANDLE PING" << std::endl;
    return (true);
}

bool handlePong(std::vector<std::string> &Token, Server &server, Client &client)
{
    (void)Token;
    (void)server;
    (void)client;
    std::cout << "HANDLE PONG" << std::endl;
    return (true);
}