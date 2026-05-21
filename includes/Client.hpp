#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string>

class Client {
    public:
		Client();
    Client(int new_socket);
		~Client();

    int getFd() const;
  
    private:
        int         _fd;
        std::string _buffer;

};

#endif