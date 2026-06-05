/*  Inviter un utilisateur
	INVITE bob #42
	Statut d'Operateur est obligatoire sur le salon ou on invite
	un autre utilisateur
*/

#include "commands/Invite.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool handleInvite(std::vector<std::string> &Token, Server &server, Client &client, bool _hasTrailing)
{
	(void)Token;
	(void)_hasTrailing;
	std::cout << "HANDLE INVITE" << std::endl;
	if (!client.getRegistration())
	{
		server.sendToClient(client.getFd(), ERR_NOTREGISTERED(client.getNickname()));
		std::cerr << "Wrong - Client is not registered" << std::endl;
		return (false);
	}
	return (true);
}