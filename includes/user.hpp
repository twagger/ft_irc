#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <iostream>
# include <stdint.h>
# include <vector>

class User {

	private:
		const int					_fd;
		std::string					_nickname;			// max 9 characters
		std::string					_username;			
		std::string 				_fullname;
		std::string					_hostname;
		uint16_t					_mode;				// OPER: SQUIT, CONNECT, KILL
		bool						_password;
		bool						_authenticated;
		std::vector<std::string>	_channelsJoined;

	public:
		User(const int fd, std::string hostname);
		User(const User &src);
		~User();

		User &operator=(User const &rhs);

		int							getFd(void) const;
		std::string 				getNickname(void) const;
		std::string 				getUsername(void) const;
		std::string 				getFullname(void) const;
		std::string 				getHostname(void) const;
		uint16_t 					getMode(void) const;
		bool 						getPassword(void) const;
		bool 						getAuthenticated(void) const;
		std::vector<std::string>	getChannelsJoined(void) const;

		void setNickname(std::string nickname);
		void setUsername(std::string username);
		void setFullname(std::string fullname);
		void setHostname(std::string fullname);
		void setMode(uint16_t mode);
		void setPassword(bool pass);
		void setAuthenticated(bool authenticated);

		bool addChannelJoined(std::string channelName);			// if channel is already in the list, return false, else add + return true 
		bool removeChannelJoined(std::string channelName);		// if channel is found in the list, erase it + return true, else do nothing and return false
};

std::ostream & operator<<(std::ostream &o, User const &rhs);	// for printing 'nickname!username@host.name.com ...'

#endif