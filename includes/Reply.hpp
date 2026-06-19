#ifndef REPLY_HPP
# define REPLY_HPP

# include <string>

// 1. Welcome Messages
#define RPL_WELCOME(nick, user, host) (std::string(":localhost 001 ") + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host)
#define RPL_YOURHOST(nick) (std::string(":localhost 002 ") + nick + " :Your host is localhost, running version 1.0")
#define RPL_CREATED(nick, date) (std::string(":localhost 003 ") + nick + " :This server was created " + date)
#define RPL_MYINFO(nick) (std::string(":localhost 004 ") + nick + " localhost 1.0 o itkol")

// 2. JOIN & TOPIC
#define RPL_NOTOPIC(nick, channel) (std::string(":localhost 331 ") + nick + " " + channel + " :No topic is set")
#define RPL_TOPIC(nick, channel, topic) (std::string(":localhost 332 ") + nick + " " + channel + " :" + topic)
#define RPL_INVITING(nick, channel, target) (std::string(":localhost 341 ") + nick + " " + target + " " + channel)
#define RPL_NAMREPLY(nick, symbol, channel, usersList) (std::string(":localhost 353 ") + nick + " " + symbol + " " + channel + " :" + usersList)
#define RPL_ENDOFNAMES(nick, channel) (std::string(":localhost 366 ") + nick + " " + channel + " :End of NAMES list")

// 3. General Errors + Auth
#define ERR_NOSUCHNICK(nick, target) (std::string(":localhost 401 ") + nick + " " + target + " :No such nick/channel")
#define ERR_NOTREGISTERED(nick) (std::string(":localhost 451 ") + nick + " :You have not registered")
#define ERR_NEEDMOREPARAMS(nick, command) (std::string(":localhost 461 ") + nick + " " + command + " :Not enough parameters")
#define ERR_ALREADYREGISTRED(nick) (std::string(":localhost 462 ") + nick + " :Unauthorized command (already registered)")
#define ERR_PASSWDMISMATCH(nick) (std::string(":localhost 464 ") + nick + " :Password incorrect")
#define ERR_UNKNOWNCOMMAND(command) (std::string(":localhost 421 ") + command + " :Unknown command")

// 4. Nickname Errors
#define ERR_NONICKNAMEGIVEN(nick) (std::string(":localhost 431 ") + nick + " :No nickname given")
#define ERR_ERRONEUSNICKNAME(nick, bad_nick) (std::string(":localhost 432 ") + nick + " " + bad_nick + " :Erroneous nickname")
#define ERR_NICKNAMEINUSE(nick, bad_nick) (std::string(":localhost 433 ") + nick + " " + bad_nick + " :Nickname is already in use")

// 5. Channel
#define ERR_NOSUCHCHANNEL(nick, channel) (std::string(":localhost 403 ") + nick + " " + channel + " :No such channel")
#define ERR_NOTONCHANNEL(nick, channel) (std::string(":localhost 442 ") + nick + " " + channel + " :You're not on that channel")
#define ERR_USERONCHANNEL(nick, target, channel) (std::string(":localhost 443 ") + nick + " " + target + " " + channel + " :is already on channel")
#define ERR_USERNOTINCHANNEL(nick, target, channel) (std::string(":localhost 441 ") + nick + " " + target + " " + channel + " :They aren't on that channel")
#define ERR_CHANNELISFULL(nick, channel) (std::string(":localhost 471 ") + nick + " " + channel + " :Cannot join channel (+l)")
#define ERR_INVITEONLYCHAN(nick, channel) (std::string(":localhost 473 ") + nick + " " + channel + " :Cannot join channel (+i)")
#define ERR_BADCHANNELKEY(nick, channel) (std::string(":localhost 475 ") + nick + " " + channel + " :Cannot join channel (+k)")
#define RPL_CHANNELMODEIS(nick, channel, modes) (std::string(":localhost 324 ") + nick + " " + channel + " " + modes)

// 6. Permission Errors
#define ERR_CHANOPRIVSNEEDED(nick, channel) (std::string(":localhost 482 ") + nick + " " + channel + " :You're not channel operator")

// 7. Messages
#define ERR_NORECIPIENT() (std::string(":localhost 411 ") + ":No recipient")
#define ERR_NOTEXTTOSEND() (std::string(":localhost 412 ") + ":No text to send")
#define RPL_TEXTTOSEND(sender_nick, user, nick, messagetext) (std::string(":" + sender_nick + "!" + user + "@localhost ") + "PRIVMSG " + nick + " :" + messagetext)
#define ERR_NOORIGIN() (std::string(":localhost 409 ") + ":No origin specified")
#define RPL_KICK(sender_nick, sender_user, channel, target, reason) (std::string(":" + sender_nick + "!" + sender_user + "@localhost ") + "KICK " + channel + " " + target + " :" + reason)
#define RPL_QUIT(sender_nick, sender_user, reason) (std::string(":" + sender_nick + "!" + sender_user + "@localhost ") + "QUIT :" + reason)
#define INVITE_MSG(sender_nick, sender_user, target_nick, channel) (std::string(":" + sender_nick + "!" + sender_user + "@localhost ") + "INVITE " + target_nick + " :" + channel)
#define ERR_CANNOTSENDTOCHAN(nick, channel) (std::string(":localhost 404 ") + nick + " " + channel + " :Cannot send to channel")

// 8. Mode Errors
#define ERR_UNKNOWNMODE(channel) (std::string(":localhost 472 ") + channel + ":Unknown mode")

#endif