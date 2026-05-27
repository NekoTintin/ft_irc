#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include "commands/Invite.hpp"
#include "commands/Join.hpp"
#include "commands/Kick.hpp"
#include "commands/Mode.hpp"
#include "commands/Nick.hpp"
#include "commands/Part.hpp"
#include "commands/Pass.hpp"
#include "commands/PingPong.hpp"
#include "commands/Privmsg.hpp"
#include "commands/Quit.hpp"
#include "commands/Topic.hpp"
#include "commands/User.hpp"

class Server;
class Client;

typedef enum e_type {
	DEFAULT,
	INVITE,
	JOIN,
	KICK,
	MODE,
	NICK,
	PART,
	PASS,
    PING,
    PONG,
    PRIVMSG,
    QUIT,
    TOPIC,
    USER,
    ERROR,
}	t_type;

class Command {
    public:
		Command();
		~Command();
        std::vector<std::string> Tokenize(std::string line);
        int processLine(std::string line, Server &server, Client &client);
        bool isaCommand(std::string tocheck);
        int findCommandIndex(std::string _command);
        int commandType(int i);
        bool selectHandler(int i, std::vector<std::string> Token, Server &server, Client &client);

    private:
        std::string                 _command;
        std::vector<std::string>    _args;
        std::vector<std::string>    _standardCommands;
};

#endif