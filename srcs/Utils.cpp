#include "Utils.hpp"

bool nonalphanumchar(char c)
{
    std::string allowedchar;

    allowedchar = "_-[]\\`^{}";
    for (size_t i = 0; i < allowedchar.size(); i++)
    {
        if (allowedchar[i] == c)
            return (true);
    }
    return (false);
}

bool correctnamenickname(std::string nametocheck)
{

    if (!isalpha(nametocheck[0]))
        return (false);
    size_t i = 0;
    while (i < nametocheck.size())
    {
        if (!isalnum(nametocheck[i]) 
            && nonalphanumchar(nametocheck[i]) == false)
                return (false);
        i++;
    }
    return (true);
}

bool correctname(std::string nametocheck)
{

    size_t i = 0;
    while (i < nametocheck.size())
    {
        if (!isalnum(nametocheck[i]) 
            && nonalphanumchar(nametocheck[i]) == false)
                return (false);
        i++;
    }
    return (true);
}