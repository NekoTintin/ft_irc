#pragma once

#include <string>

// 1. Basics (Registration)
#define RPL_WELCOME(nick, user, host) (std::string("001 ") + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host)
#define RPL_YOURHOST(nick) (std::string("002 ") + nick + " :Your host is localhost, running version 1.0")
#define RPL_CREATED(nick, date) (std::string("003 ") + nick + " :This server was created " + date)
#define RPL_MYINFO(nick) (std::string("004 ") + nick + " localhost 1.0 o itkol")

// 2. JOIN & TOPIC
#define RPL_NOTOPIC(nick, channel) (std::string("331 ") + nick + " " + channel + " :No topic is set")
#define RPL_TOPIC(nick, channel, topic) (std::string("332 ") + nick + " " + channel + " :" + topic)
#define RPL_INVITING(nick, channel, target) (std::string("341 ") + nick + " " + target + " " + channel)
#define RPL_NAMREPLY(nick, symbol, channel, usersList) (std::string("353 ") + nick + " " + symbol + " " + channel + " :" + usersList)
#define RPL_ENDOFNAMES(nick, channel) (std::string("366 ") + nick + " " + channel + " :End of /NAMES list")

// 3. General Errors + Auth
#define ERR_NOSUCHNICK(nick, target) (std::string("401 ") + nick + " " + target + " :No such nick/channel")
#define ERR_NOTREGISTERED(nick) (std::string("451 ") + nick + " :You have not registered")
#define ERR_NEEDMOREPARAMS(nick, command) (std::string("461 ") + nick + " " + command + " :Not enough parameters")
#define ERR_ALREADYREGISTRED(nick) (std::string("462 ") + nick + " :Unauthorized command (already registered)")
#define ERR_PASSWDMISMATCH(nick) (std::string("464 ") + nick + " :Password incorrect")

// 4. Nickname Errors
#define ERR_NONICKNAMEGIVEN(nick) (std::string("431 ") + nick + " :No nickname given")
#define ERR_ERRONEUSNICKNAME(nick, bad_nick) (std::string("432 ") + nick + " " + bad_nick + " :Erroneous nickname")
#define ERR_NICKNAMEINUSE(nick, bad_nick) (std::string("433 ") + nick + " " + bad_nick + " :Nickname is already in use")

// 5. Channel Errors
#define ERR_NOSUCHCHANNEL(nick, channel) (std::string("403 ") + nick + " " + channel + " :No such channel")
#define ERR_NOTONCHANNEL(nick, channel) (std::string("442 ") + nick + " " + channel + " :You're not on that channel")
#define ERR_USERONCHANNEL(nick, target, channel) (std::string("443 ") + nick + " " + target + " " + channel + " :is already on channel")
#define ERR_CHANNELISFULL(nick, channel) (std::string("471 ") + nick + " " + channel + " :Cannot join channel (+l)")
#define ERR_INVITEONLYCHAN(nick, channel) (std::string("473 ") + nick + " " + channel + " :Cannot join channel (+i)")
#define ERR_BADCHANNELKEY(nick, channel) (std::string("475 ") + nick + " " + channel + " :Cannot join channel (+k)")

// 6. Permission Errors
#define ERR_CHANOPRIVSNEEDED(nick, channel) (std::string("482 ") + nick + " " + channel + " :You're not channel operator")