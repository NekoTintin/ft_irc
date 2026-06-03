#pragma once

#include <string>

// 1. Welcome Messages
#define RPL_WELCOME(nick, user, host) (std::string(":localhost 001 ") + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")
#define RPL_YOURHOST(nick) (std::string(":localhost 002 ") + nick + " :Your host is localhost, running version 1.0\r\n")
#define RPL_CREATED(nick, date) (std::string(":localhost 003 ") + nick + " :This server was created " + date + "\r\n")
#define RPL_MYINFO(nick) (std::string(":localhost 004 ") + nick + " localhost 1.0 o itkol\r\n")

// 2. JOIN & TOPIC
#define RPL_NOTOPIC(nick, channel) (std::string(":localhost 331 ") + nick + " " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(nick, channel, topic) (std::string(":localhost 332 ") + nick + " " + channel + " :" + topic + "\r\n")
#define RPL_INVITING(nick, channel, target) (std::string(":localhost 341 ") + nick + " " + target + " " + channel + "\r\n")
#define RPL_NAMREPLY(nick, symbol, channel, usersList) (std::string(":localhost 353 ") + nick + " " + symbol + " " + channel + " :" + usersList + "\r\n")
#define RPL_ENDOFNAMES(nick, channel) (std::string(":localhost 366 ") + nick + " " + channel + " :End of /NAMES list\r\n")

// 3. General Errors + Auth
#define ERR_NOSUCHNICK(nick, target) (std::string(":localhost 401 ") + nick + " " + target + " :No such nick/channel\r\n")
#define ERR_NOTREGISTERED(nick) (std::string(":localhost 451 ") + nick + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(nick, command) (std::string(":localhost 461 ") + nick + " " + command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED(nick) (std::string(":localhost 462 ") + nick + " :Unauthorized command (already registered)\r\n")
#define ERR_PASSWDMISMATCH(nick) (std::string(":localhost 464 ") + nick + " :Password incorrect\r\n")

// 4. Nickname Errors
#define ERR_NONICKNAMEGIVEN(nick) (std::string(":localhost 431 ") + nick + " :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nick, bad_nick) (std::string(":localhost 432 ") + nick + " " + bad_nick + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nick, bad_nick) (std::string(":localhost 433 ") + nick + " " + bad_nick + " :Nickname is already in use\r\n")

// 5. Channel Errors
#define ERR_NOSUCHCHANNEL(nick, channel) (std::string(":localhost 403 ") + nick + " " + channel + " :No such channel\r\n")
#define ERR_NOTONCHANNEL(nick, channel) (std::string(":localhost 442 ") + nick + " " + channel + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(nick, target, channel) (std::string(":localhost 443 ") + nick + " " + target + " " + channel + " :is already on channel\r\n")
#define ERR_CHANNELISFULL(nick, channel) (std::string(":localhost 471 ") + nick + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(nick, channel) (std::string(":localhost 473 ") + nick + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(nick, channel) (std::string(":localhost 475 ") + nick + " " + channel + " :Cannot join channel (+k)\r\n")

// 6. Permission Errors
#define ERR_CHANOPRIVSNEEDED(nick, channel) (std::string(":localhost 482 ") + nick + " " + channel + " :You're not channel operator\r\n")