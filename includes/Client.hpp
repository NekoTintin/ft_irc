#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string>

class Client {
    public:
		Client();
    Client(int new_socket);
		~Client();

    int         getFd() const;
    void        addToBuffer(const std::string &data);
    bool        isComplete();
    std::string getCommand();
    void        removeCommand();

    bool        getCorrectPassword();
    void        setCorrectPassword();

    bool        getRegistration();
    void        setRegistration();

    std::string getNickname();
    void        setNickname(std::string nickame);
    bool        getNicknamestatus();
  
    private:
        int          _fd;
        std::string _buffer;
        bool        _registration;
        bool        _correctPassword;
        std::string _nickname;
        bool        _nicknameset;

};

#endif