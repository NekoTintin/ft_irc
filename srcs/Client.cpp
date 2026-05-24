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

bool Client::isComplete()
{
    std::size_t found = _buffer.find("\r\n");
    if (found != std::string::npos)
        return (true);
    return (false);
}
std::string Client::getCommand()
{
    std::size_t found = _buffer.find("\r\n");
    std::string complete_command = _buffer.substr(0, found);
    return (complete_command);
}
void Client::removeCommand()
{
    std::size_t found = _buffer.find("\r\n");
    _buffer.erase(0, found + 2);
}