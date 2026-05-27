#include "Command.hpp"

Command::Command()
{
    _standardCommands.push_back("INVITE");
    _standardCommands.push_back("JOIN");
    _standardCommands.push_back("KICK");
    _standardCommands.push_back("MODE");
    _standardCommands.push_back("NICK");
    _standardCommands.push_back("PART");
    _standardCommands.push_back("PASS");
    _standardCommands.push_back("PING");
    _standardCommands.push_back("PONG");
    _standardCommands.push_back("PRIVMSG");
    _standardCommands.push_back("QUIT");
    _standardCommands.push_back("TOPIC");
    _standardCommands.push_back("USER");
}

Command::~Command() {};

bool Command::isaCommand(std::string tocheck)
{
    size_t i = 0;
    while (i < _standardCommands.size())
    {
        if (tocheck == _standardCommands[i])
            return (true);
        i++;
    }
    return (false);
}
int Command::findCommandIndex(std::string _command)
{
    size_t i = 0; 

    while (i < _standardCommands.size())
    {
        if (_command == _standardCommands[i])
            return (i+1);
        i++;
    } 
    return (0);
}

int Command::commandType(int i)
{
    int type = 0;
    switch (i)
    {
        case 0: type = DEFAULT; break;
        case 1: type = INVITE; break;
        case 2: type = JOIN; break;
        case 3: type = KICK; break;
        case 4: type = MODE; break;
        case 5: type = NICK; break;
        case 6: type = PART; break;
        case 7: type = PASS; break;
        case 8: type = PING; break;
        case 9: type = PONG; break;
        case 10: type = PRIVMSG; break;
        case 11: type = QUIT; break;
        case 12: type = TOPIC; break;
        case 13: type = USER; break;
    }
    return (type);
}

bool Command::selectHandler(int i, std::vector<std::string> Token, Server &server, Client &client)
{
    switch (i)
    {
        case 0: return (false);
        case 1: handleInvite(); break;
        case 2: handleJoin(); break;
        case 3: handleKick(); break;
        case 4: handleMode(); break;
        case 5: 
            if (handleNick(Token, server, client) == true) break;
            return (false);
            
        case 6: handlePart(); break;
        case 7: 
            if (handlePass(Token, server, client) == true) break;
            return (false);
        case 8: handlePing(); break;
        case 9: handlePong(); break;
        case 10: handlePrivmsg(); break;
        case 11: handleQuit(); break;
        case 12: handleTopic(); break;
        case 13: handleUser(); break;
    }
    return(true);
}

std::vector<std::string> Command::Tokenize(std::string line)
{
    size_t                      i = 0;
    size_t                      j = 0;
    size_t                      begin_word = 0;
    std::string                 temp_str;
    std::vector<std::string>    token;

    while (i < line.size())
    {
        while (line[i] == ' ')
            i++;
        begin_word = i;
        if (line[i] == ':')
        {
            while (i < line.size())
            {
                j++;
                i++;
            }
            temp_str = line.substr(begin_word + 1, j - 1);
            token.push_back(temp_str);
            temp_str.erase();
            break;
        }
        while (i < line.size() && line[i] != ' ')
        {
            j++;
            i++;
        }
        temp_str = line.substr(begin_word, j);
        token.push_back(temp_str);
        temp_str.erase();
        j = 0;
    }
    return (token);
}

int Command::processLine(std::string line, Server &server, Client &client)
{
    std::vector<std::string>    token = Tokenize(line);
    // int                         handler = 0;

    // for (size_t i = 0; i < token.size(); i++)
    //     std::cout << "token : [" << token[i] << "] " << std::endl;

    if (!isaCommand(token[0]))
        return (ERROR);
    else
    {
        _command = token[0];
        for (size_t i = 1; i < token.size(); i++)
            _args.push_back(token[i]);
    }

    selectHandler(commandType(findCommandIndex(_command)), token, server, client);
    // std::cout << "handler = " << handler << std::endl;

    // std::cout << "_command : [" << _command << "] " << std::endl;

    // for (size_t i = 0; i < _args.size(); i++)
    //     std::cout << "args : [" << _args[i] << "] " << std::endl;

    return (0);
}



