#include "Command.hpp"

Command::Command()
{
	_standardCommands.push_back("CAP");
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
		case 1: type = CAP; break;
		case 2: type = INVITE; break;
		case 3: type = JOIN; break;
		case 4: type = KICK; break;
		case 5: type = MODE; break;
		case 6: type = NICK; break;
		case 7: type = PART; break;
		case 8: type = PASS; break;
		case 9: type = PING; break;
		case 10: type = PONG; break;
		case 11: type = PRIVMSG; break;
		case 12: type = QUIT; break;
		case 13: type = TOPIC; break;
		case 14: type = USER; break;
	}
	return (type);
}

bool Command::selectHandler(int i, std::vector<std::string> Token, Server &server, Client &client)
{
	switch (i)
	{
		case 0: return (false);
		case 1: return (handleCap(Token, server, client));
		case 2: return (handleInvite(Token, server, client));
		case 3: return (handleJoin(Token, server, client));
		case 4: return (handleKick(Token, server, client));
		case 5: return (handleMode(Token, server, client));
		case 6: return (handleNick(Token, server, client));
		case 7: return (handlePart(Token, server, client));
		case 8: return (handlePass(Token, server, client));
		case 9: return (handlePing(Token, server, client));
		case 10: return (handlePong(Token, server, client));
		case 11: return (handlePrivmsg(Token, server, client));
		case 12: return (handleQuit(Token, server, client));
		case 13: return (handleTopic(Token, server, client));
		case 14: return (handleUser(Token, server, client));
		case 15: return (false);
	}
	return (true);
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
		while (i < line.size() && line[i] == ' ')
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
	if (line.empty() && line[line.length() - 1] != '\n')
		return (ERROR);
	if (line.empty() && line[line.length() - 1] != '\r')
		return (ERROR);

	_command.clear();
	_args.clear();

	std::vector<std::string>    token = Tokenize(line);
	if (token.empty())
		return (ERROR);
	if (!isaCommand(token[0]))
		return (ERROR);
	_command = token[0];
	for (size_t i = 1; i < token.size(); i++)
		_args.push_back(token[i]);

	int command_index = findCommandIndex(_command);
	selectHandler(commandType(command_index), token, server, client);

	return (0);
}