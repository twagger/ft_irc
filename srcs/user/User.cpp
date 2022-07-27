#include "../../includes/User.hpp"
#include <ctime>

// CONSTRUCTORS
User::User(const int fd, std::string hostname) : _fd(fd), _nickname("*"), 
			_hostname(hostname), _mode(0), _authenticated(false), _channelsJoined(), 
			_status(ST_ALIVE), _lastActivityTime(time(NULL)){ }

User::User(const User &src) : _fd(src._fd), _status(ST_ALIVE), _lastActivityTime(time(NULL)){
	*this = src;
}

// DESTRUCTORS
User::~User() { }

// ASSIGN OVERLOAD
User& User::operator=(User const &rhs) {
	if (this != &rhs) {
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
		this->_fullname = rhs._fullname;
		this->_hostname = rhs._hostname;
		this->_mode = rhs._mode;
		this->_password = rhs._password;
		this->_authenticated = rhs._authenticated;
		this->_channelsJoined = rhs._channelsJoined;
	}
	return *this;
}

// GETTERS
int							User::getFd(void) const { return this->_fd; }
std::string					User::getNickname(void) const { return this->_nickname; }
std::string					User::getUsername(void) const { return this->_username; }
std::string					User::getFullname(void) const { return this->_fullname; }
std::string					User::getHostname(void) const { return this->_hostname; }
uint8_t					    User::getMode(void) const { return this->_mode; }
bool 						User::getPassword(void) const { return this->_password; }
bool 						User::getAuthenticated(void) const {
	return this->_authenticated;
}
std::vector<std::string>	User::getChannelsJoined(void) const {
	return this->_channelsJoined;
}
int							User::getStatus(void) const { return this->_status; }
time_t  					User::getLastActivityTime(void) const { return this->_lastActivityTime; }
time_t  					User::getPingTime(void) const { return this->_pingTime; }
bool                        User::hasMode(uint8_t mode) { return ((this->_mode & mode) > 0); }

// SETTERS
void User::setNickname(std::string nickname) { this->_nickname = nickname; }
void User::setUsername(std::string username) { this->_username = username; }
void User::setFullname(std::string fullname) { this->_fullname = fullname; }
void User::setHostname(std::string hostname) { this->_hostname = hostname; }
void User::setPassword(bool password) { this->_password = password; }
void User::setAuthenticated(bool authenticated) { 
	this->_authenticated = authenticated; 
}
void User::setStatus(int status) { this->_status = status; }
void User::setLastActivityTime(void) { this->_lastActivityTime = time(NULL); }
void User::setPingTime(void) { this->_pingTime = time(NULL); }
void User::addMode(uint8_t mode) { this->_mode |= mode; }
void User::removeMode(uint8_t mode) { this->_mode &= ~mode; }

// CHANNEL JOINED MANAGEMENT
bool User::addChannelJoined(std::string channelName) {
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = this->_channelsJoined.end();

	for (it = this->_channelsJoined.begin(); it < ite; ++it) {
		if (*it == channelName)
			return false;
	}
	this->_channelsJoined.push_back(channelName);
	return true;
}

bool User::removeChannelJoined(std::string channelName) {
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = this->_channelsJoined.end();

	for (it = this->_channelsJoined.begin(); it < ite; ++it) {
		if (*it == channelName) {
			this->_channelsJoined.erase(it);
			return true;
		}
	}
	return false;
}

// OSTREAM 
std::ostream & operator<<(std::ostream &o, User const &rhs) {
	if (rhs.getAuthenticated() == true)
		o << rhs.getNickname() << "!" << rhs.getUsername();
	return o;
}
