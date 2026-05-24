#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

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
        int processLine(std::string line);
        bool isaCommand(std::string tocheck);
        int findCommandIndex(std::string _command);
        int commandType(int i);

    private:
        std::string                 _command;
        std::vector<std::string>    _args;
        std::vector<std::string>    _standardCommands;
};

#endif