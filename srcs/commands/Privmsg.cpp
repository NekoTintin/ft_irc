/* Envoyer un message prive ou dans un channel
    ex : 
        PRIVMSG utilisateur :hello
        PRIVMSG # :hello
*/

#include "commands/Privmsg.hpp"

bool handlePrivmsg()
{
    std::cout << "HANDLE PRIVATE MESSAGE" << std::endl;
    return (true);
}