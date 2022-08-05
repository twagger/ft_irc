#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <iostream>
# include <vector>
# include <deque>
# include <ctime>

# define ST_ALIVE 1
# define ST_DEAD 0
# define ST_PINGED 2

//   The available modes are:
//		none												=> 0	0000 0000
//		a - user is flagged as away;						=> 1	0000 0001
//		w - user receives wallops;							=> 2	0000 0010			
//     	i - marks a users as invisible; 					=> 3	0000 0100
//      o - operator flag;  								=> 4	0000 1000
//      B - marks a bot;									=> 5	0001 0000
//      s - marks a user for receipt of server notices.		=> 6	0010 0000
//      r - restricted user connection;	 					=> 7	0100 0000

#define MOD_NONE        (0 << 0)
#define MOD_AWAY        (1 << 0)
#define MOD_WALLOPS     (1 << 2)
#define MOD_INVISIBLE   (1 << 3)
#define MOD_OPER        (1 << 4)
#define MOD_BOT    		(1 << 5)
#define MOD_SRVNOTICES  (1 << 6)
#define MOD_RESTRICTED  (1 << 7)

class User {

	private:
		const int					_fd;
		std::string					_nickname;			// max 9 characters
		std::string					_username;			
		std::string 				_fullname;
		std::string					_hostname;
		short					    _mode;
		bool						_password;
		bool						_authenticated;
		std::deque<std::string>		_channelsJoined;
		int							_status;
        time_t                      _lastActivityTime;
        time_t                      _pingTime;
		bool						_isBot;


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
		short 					getMode(void) const;
		bool 						getPassword(void) const;
		bool 						getAuthenticated(void) const;
		std::deque<std::string>		getChannelsJoined(void) const;
		int							getStatus(void) const;
        time_t  					getLastActivityTime(void) const;
        time_t  					getPingTime(void) const;
		bool 						getIsBot(void) const;

		void setNickname(std::string nickname);
		void setUsername(std::string username);
		void setFullname(std::string fullname);
		void setHostname(std::string fullname);
		void setPassword(bool pass);
		void setAuthenticated(bool authenticated);
		void setStatus(int status);
		void setLastActivityTime(void);
		void setPingTime(void);
		void setIsBot(bool bot);

        // mode
		void addMode(short mode);
		void removeMode(short mode);
        bool hasMode(short mode);

		bool addChannelJoined(std::string channelName);			// if channel is already in the list, return false, else add + return true 
		bool removeChannelJoined(std::string channelName);		// if channel is found in the list, erase it + return true, else do nothing and return false
};

std::ostream & operator<<(std::ostream &o, User const &rhs);	// for printing 'nickname!username@host.name.com ...'

#endif
