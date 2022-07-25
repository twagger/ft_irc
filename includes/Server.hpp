#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <map>
#include <set>
#include <ctime>
#include "channel.hpp"
#include "user.hpp"
#include "usercmds.hpp"

#define MAX_CMD_LEN 512
#define BACKLOG 10
#define BUF_SIZE 4096
#define MAX_EVENTS 10
#define PING_DELAY 5 // in minutes
#define	PING(hostname) ("PING " + hostname + "\r\n")

struct Command
{
    std::string                 command;
    std::string                 prefix;
    std::vector<std::string>    params;

    Command(std::string cmd, std::string prefix = std::string(), \
            std::vector<std::string> params = std::vector<std::string>());
};

class Server
{
    public:
        // member type
        typedef \
            std::string (*CmdFunction)(int, std::vector<std::string>, Server*); 

        // Constructors & destructor
        Server(int port, std::string password, std::string name = "Gunther");
        Server(Server const &src);
        ~Server(void){};

        // Operator overload
        Server  &operator=(Server const &rhs);
        
        // Getters
		int			getPort(void) const;
		std::string	getPassword(void) const;
		std::string	getName(void) const;
		std::string getHostname(void) const;
		std::string getVersion(void) const;
		std::string getDate(void) const;
		User*		getUserByFd(const int &fd) const;
        User*		getUserByNickname(const std::string &nick) const;

        // Member functions
        void    start(void);
        void    sendClient(int fd, std::string message) const;
        void    sendClient(std::set<int> &fds, std::string message) const;
        void    broadcast(std::string message) const;
        void    sendChannel(std::string channel, std::string message) const;
        void    killConnection(int fd);

        // exceptions
        class socketException : public std::exception
        { public: virtual const char *what() const throw(); };
        
        class bindException : public std::exception
        { public: virtual const char *what() const throw(); };

        class pollException : public std::exception
        { public: virtual const char *what() const throw(); };

        class pollWaitException : public std::exception
        { public: virtual const char *what() const throw(); };

        class pollAddException : public std::exception
        { public: virtual const char *what() const throw(); };

        class pollDelException : public std::exception
        { public: virtual const char *what() const throw(); };

        class acceptException : public std::exception
        { public: virtual const char *what() const throw(); };
        
        class passwordException : public std::exception
        { public: virtual const char *what() const throw(); };

        class invalidFdException : public std::exception
        { public: virtual const char *what() const throw(); };

        class invalidChannelException : public std::exception
        { public: virtual const char *what() const throw(); };

        class sendException : public std::exception
        { public: virtual const char *what() const throw(); };

        class readException : public std::exception
        { public: virtual const char *what() const throw(); };
 
        std::map<std::string, Channel *>    _channelList;
        std::map<std::string, CmdFunction>  _cmdList;
        std::set<std::string>               _unavailableNicknames;

    private:
        // Cannot be default construct
        Server(void){};

        // Private member functions
        void    _initCommandList(void);
        int     _createSocket(void);
        void    _bindSocket(int sockfd, struct sockaddr_in *srv_addr);
        int     _createPoll(int sockfd);
        int     _pollWait(int pollfd, struct epoll_event **events, \
                           int max_events);
        void    _acceptConnection(int sockfd, int pollfd);
        void    _handleNewMessage(struct epoll_event event);
        void    _executeCommands(int fd, std::vector<Command> cmds);
        void    _pingClients(void);

        // Member attributes
        int                     _port;
        std::string             _password;
        std::string             _name;
        std::string             _hostname;
		std::string             _version;
		std::string             _date;			// expected format: 19:52:09 Aug 12 2013
        time_t                  _lastPingTime;

        int                     _pollfd;
        int                     _sockfd;

        std::map<const int, User *>   _userList;

		// <hostname> has a maximum length of 63 characters. If name is longer than 63
        // characters are registered using the host (numeric) address instead of the host name.
		// hostname   =  shortname *( "." shortname )
		// hostaddr   =  ip4addr / ip6addr
};

#endif
