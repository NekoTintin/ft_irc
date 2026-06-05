#include "Utils.hpp"

bool nonalphanumchar(char c) {
	std::string allowedchar;

	allowedchar = "_-[]\\|`^{}";
	for (size_t i = 0; i < allowedchar.size(); i++) {
		if (allowedchar[i] == c)
			return (true);
	}
	return (false);
}

bool correctnamenickname(std::string nametocheck) {

	if (!isalpha(nametocheck[0]))
		return (false);
	size_t i = 0;
	while (i < nametocheck.size()) {
		if (!isalnum(nametocheck[i]) 
			&& nonalphanumchar(nametocheck[i]) == false)
				return (false);
		i++;
	}
	return (true);
}

bool correctname(std::string nametocheck) {
	size_t i = 0;
	while (i < nametocheck.size()) {
		if (!isalnum(nametocheck[i]) 
			&& nonalphanumchar(nametocheck[i]) == false)
				return (false);
		i++;
	}
	return (true);
}

std::string getDate() {
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);

	char buf[80];
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localTime);
	
	return std::string(buf);
}