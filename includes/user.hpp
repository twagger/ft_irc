#ifndef USER_HPP
# define USER_HPP

# include <string>

class User {

	private:
		std::string	_nickname;
		std::string	_username;
		std::string _fullname;

	public:
		User(void);
		User(const User &other) {
			*this = other;
		}
		~User();

		User &operator=(const User &src) {
			if (*this != &src) {
				*this->_nickname = src._nickname;
				*this->_username = src._username;
				*this->_fullname = src._fullname;
			}
			return *this;
		}

		std::string getNickname(void) { return this->_nickname; }
		std::string getUsername(void) { return this->_username; }
		std::string getFullname(void) { return this->_fullname; }

		setNickname(std::string nickname) : { this->_nickname = nickname; }
		setUsername(std::string username) : { this->_username = username; }
		setFullname(std::string username) : { this->_fullname = fullname; }

}

#endif