#include "Client.hpp"

Client::Client()
{
    this->_registration = false;
    this->_correctPassword = false;
    this->_nicknameset = false;
    this->_usernameset = false;
    this-> _realnameset = false;
};

Client::~Client() {};

Client::Client(int new_socket){
    this->_fd = new_socket;
    this->_registration = false;
    this->_correctPassword = false;
    this->_nicknameset = false;
    this->_usernameset =false;
    this-> _realnameset = false;
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

bool Client::getCorrectPassword()
{
    return (_correctPassword);
}

void Client::setCorrectPassword()
{
    _correctPassword = true;
}

bool Client::getRegistration()
{
    return (_registration);
}

void Client::setRegistration()
{
    _registration = true;
}

void Client::setNickname(std::string nickame)
{
    _nickname = nickame;
    _nicknameset = true;
}

bool Client::getNicknamestatus()
{
    return (_nicknameset);
}

std::string Client::getNickname()
{
    return (_nickname);
}

std::string Client::getUsername()
{
    return (_username);
}
void Client::setUsername(std::string username)
{
    _username = username;
    _usernameset = true;
}

bool Client::getUsernamestatus()
{
    return (_usernameset);
}

std::string Client::getRealname()
{
    return (_realname);
}
void Client::setRealname(std::string realname)
{
    _realname = realname;
    _realnameset = true;
}

bool Client::getRealnamestatus()
{
    return (_realnameset);
}
