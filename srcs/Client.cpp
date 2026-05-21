#include "Client.hpp"

Client::Client() {};

Client::~Client() {};

Client::Client(int new_socket){
    this->_fd = new_socket;
}

int Client::getFd() const{
    return (this->_fd);
}

void Client::addToBuffer(const std::string &data)
{
    this->_buffer += data;
}
