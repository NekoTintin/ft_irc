_This project has been created as part of the 42 curriculum by mlelu, qupollet._

# ft_irc

## Description

The goal of this project is to recreate a simple IRC server in C++. The server will handle multiple clients, allowing them to connect, join channels, and send messages to each other.
The project must implement the following commands:
- NICK
- INVITE
- TOPIC
- MODE
	- i
	- t
	- k
	- o
	- l

IRC must have two types of users: operators and regular users. Operators have more privileges, they can kick users, change channel modes, invite users, etc.

### Small Overview
- Server:
	- Receives the connexion requests from several clients

- Clients:
	- Each client is a user with a nickname
	- All the clients are stored in a Map which as an attribute of the Server

## Instructions

How to start:
```bash
make
./ircserv <port> <password>
```
Start Irssi :
```bash
irssi -c localhost -p <port> -w <password> -n <nickname>
```

Or with nc:
```bash
nc localhost <port>
PASS <password>
NICK <nickname>
USER <username> 0 * :<realname>
```

## Ressources

- Poll():
	- https://bousk.developpez.com/cours/reseau-c++/TCP/05-envoi-reception-serveur/
	- https://www.ibm.com/docs/fr/i/7.5.0?topic=designs-using-poll-instead-select

- Send():
	- https://pubs.opengroup.org/onlinepubs/009696599/functions/send.html
	- https://linux.die.net/man/2/send
	- https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/
	 
- Sockets:
	- https://youtu.be/gntyAFoZp-E?si=g4ap0MMGicZ-UDn7

- Recve():
	- https://pubs.opengroup.org/onlinepubs/007904975/functions/recv.html
	- https://www.ibm.com/docs/en/

- Signal :
	- https://www.geeksforgeeks.org/cpp/signal-handling-in-cpp/

- Project overview :
	- https://nathaan.me/projects/irc
	- https://modern.ircdocs.horse/
	- https://mathieu-lemoine.developpez.com/tutoriels/irc/protocole/?page=connexion

- IRC Commands :
	- https://fr.wikipedia.org/wiki/Aide:IRC/commandes
	- https://dd.ircdocs.horse/refs/commands/user
	- https://www.alien.net.au/irc/irc2numerics.html

- IRSSI :
	- https://doc.fedora-fr.org/wiki/Irssi_-_client_IRC_en_mode_console

### AI Usage

- Ai was used to help with some points of the IRC documentation, when it was unclear. Also it was used to verify some of the commands and their parameters, to make sure we were implementing them correctly.