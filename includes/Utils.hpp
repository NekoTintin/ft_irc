#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>

bool			nonalphanumchar(char c);
bool			correctname(std::string nametocheck);
bool			correctnamenickname(std::string nametocheck);
std::string		getDate();
std::string		buildTrailingMsg(std::string &message);

#endif
