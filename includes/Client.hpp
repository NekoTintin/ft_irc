#ifndef CLIENT_HPP
# define CLIENT_HPP

# define NB_CHARS 512

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

    std::string getUsername();
    void        setUsername(std::string username);
    bool        getUsernamestatus();
  
    std::string getRealname();
    void        setRealname(std::string realname);
    bool        getRealnamestatus();

    private:
        int          _fd;
        std::string _buffer;
        bool        _registration;
        bool        _correctPassword;
        std::string _nickname;
        bool        _nicknameset;
        std::string _username;
        bool        _usernameset;
        std::string _realname;
        bool        _realnameset;

};

#endif