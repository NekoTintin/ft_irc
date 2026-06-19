#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <iostream>
# include <ctime>
# include "Client.hpp"
# include "Channel.hpp"

bool			nonalphanumchar(char c);
bool			correctname(std::string nametocheck);
bool			correctnamenickname(std::string nametocheck);
std::string		getDate();

#endif