#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <iostream>
# include <stdint.h>
# include <vector>
# include <ctime>

# define ST_ALIVE 1
# define ST_DEAD 0
# define ST_PINGED 2

//   The available modes are as follows:
//		none								=> 0	000
//		a - user is flagged as away;		=> 1	001			
//     	i - marks a users as invisible; 	=> 2	010
//      w - user receives wallops; 			=> 3	011
//      r - restricted user connection; 	=> 4	100
//      o - operator flag; 					=> 5	101 
//      O - local operator flag; 			=> 6	110	
//      s - marks a user for receipt of server notices. => 7	111

class User {

	private:
		const int					_fd;
		std::string					_nickname;			// max 9 characters
		std::string					_username;			
		std::string 				_fullname;
		std::string					_hostname;
		uint16_t					_mode;
		bool						_password;
		bool						_authenticated;
		std::vector<std::string>	_channelsJoined;
		int							_status;
        time_t                      _lastActivityTime;
        time_t                      _pingTime;


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
		int							getStatus(void) const;
        time_t  					getLastActivityTime(void) const;
        time_t  					getPingTime(void) const;


		void setNickname(std::string nickname);
		void setUsername(std::string username);
		void setFullname(std::string fullname);
		void setHostname(std::string fullname);
		void setMode(uint16_t mode);
		void setPassword(bool pass);
		void setAuthenticated(bool authenticated);
		void setStatus(int status);
		void setLastActivityTime(void);
		void setPingTime(void);
		
		bool addChannelJoined(std::string channelName);			// if channel is already in the list, return false, else add + return true 
		bool removeChannelJoined(std::string channelName);		// if channel is found in the list, erase it + return true, else do nothing and return false
};

std::ostream & operator<<(std::ostream &o, User const &rhs);	// for printing 'nickname!username@host.name.com ...'

#endif
