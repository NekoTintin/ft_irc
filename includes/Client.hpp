#ifndef CLIENT_HPP
# define CLIENT_HPP

# define NB_CHARS 512

# include <iostream>
# include <string>

class Client {
	public:
		// Constructors and Destructor
		Client();
		Client(int new_socket);
		~Client();

		// FDs and Buffer
		int						getFd() const;
		void					addToBuffer(const std::string &data);
		bool					isComplete();
		std::string				getCommand();
		void					removeCommand();

		// Password
		bool					getCorrectPassword();
		void					setCorrectPassword();

		// Registration status
		bool					getRegistration();
		void					setRegistration();

		// Nickname
		const std::string		&getNickname() const;
		void					setNickname(std::string nickame);
		bool					getNicknamestatus();

		// Username
		const std::string		&getUsername() const;
		void					setUsername(std::string username);
		bool					getUsernamestatus();

		// Realname
		std::string				getRealname();
		void					setRealname(std::string realname);
		bool					getRealnamestatus();

	private:
		int						_fd;
		std::string				_buffer;
		bool					_registration;
		bool					_correctPassword;
		std::string				_nickname;
		bool					_nicknameset;
		std::string				_username;
		bool					_usernameset;
		std::string				_realname;
		bool					_realnameset;
};

#endif