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
    void addToBuffer(const std::string &data);
    bool isComplete();
    std::string getCommand();
    void removeCommand();
  
    private:
        int         _fd;
        std::string _buffer;

};

#endif