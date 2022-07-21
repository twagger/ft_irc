#include "../../includes/user.hpp"

// CONSTRUCTORS
User::User(int fd, std::string hostname) : _fd(fd), _nickname("*"), _hostname(hostname), _mode(0), _authenticated(false) { }

User::User(const User &src) : _fd(src._fd) {
	*this = src;
}

// DESTRUCTORS
User::~User() { }

// ASSIGN OVERLOAD
User& User::operator=(User const &rhs) {
	if (this != &rhs) {
		this->_fd = rhs._fd;
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
		this->_fullname = rhs._fullname;
		this->_hostname = rhs._hostname;
		this->_mode = rhs._mode;
		this->_password = rhs._password;
		this->_authenticated = rhs._authenticated;
	}
	return *this;
}

// GETTERS
std::string	User::getNickname(void) const { return this->_nickname; }
std::string	User::getUsername(void) const { return this->_username; }
std::string	User::getFullname(void) const { return this->_fullname; }
std::string	User::getHostname(void) const { return this->_hostname; }
uint16_t	User::getMode(void) const { return this->_mode; }
bool 		User::getPassword(void) const { return this->_password; }
bool 		User::getAuthenticated(void) const { return this->_authenticated; }

// SETTERS
void User::setNickname(std::string nickname) { this->_nickname = nickname; }
void User::setUsername(std::string username) { this->_username = username; }
void User::setFullname(std::string fullname) { this->_fullname = fullname; }
void User::setHostname(std::string hostname) { this->_hostname = hostname; }
void User::setMode(uint16_t mode) { this->_mode = mode; }
void User::setPassword(bool password) { this->_password = password; }
void User::setAuthenticated(bool authenticated) { this->_authenticated = authenticated; }

// OSTREAM 
std::ostream & operator<<(std::ostream &o, User const &rhs) {
	if (rhs.getAuthenticated() == true)
		o << rhs.getNickname() << "!" << rhs.getUsername();
	return o;
}
