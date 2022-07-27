#ifndef SERVER_HPP
# define SERVER_HPP

// Standard headers
#include <vector>
#include <map>
#include <set>
#include <ctime>

// Custom headers
#include "channel.hpp"
#include "user.hpp"
#include "usercmds.hpp"

// Sockets & packets params
#define MAX_CMD_LEN 512
#define BACKLOG 10
#define BUF_SIZE 4096
#define MAX_EVENTS 10

// Client check params
#define PING_TIMEOUT 120 // in seconds
#define PONG_TIMEOUT 20 // in seconds
#define WAIT_TIMEOUT 3000 // in milliseconds

// Program infos
#ifndef VERSION
# define VERSION "1"
#endif
#ifndef VCOMMENT
# define VCOMMENT "No comment"
#endif
#ifndef COMPILDATE
# define COMPILDATE "Sun 25 Sep 1983 10:00:00 AM CEST"
#endif

// Non numeric replies
#define	PING(hostname) (":" + hostname + " PING " + hostname + "\r\n")
#define	PONG(hostname) (":" + hostname + " PONG " + hostname + "\r\n")

// Utility structure
struct Command
{
    std::string                 command;
    std::string                 prefix;
    std::vector<std::string>    params;

    Command(std::string cmd, std::string prefix = std::string(), \
            std::vector<std::string> params = std::vector<std::string>());
};

// Server class
class Server
{
    public:
        // member type
        typedef \
            const std::string (*CmdFunction)(const int &, \
            const std::vector<std::string> &, const std::string &, Server*); 

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
 
        std::map<std::string, Channel *>   _channelList;
        std::map<std::string, CmdFunction> _cmdList;
        std::map<std::string, time_t>      _unavailableNicknames;

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

        int                     _pollfd;
        int                     _sockfd;

        std::map<const int, User *>   _userList;

		// <hostname> has a maximum length of 63 characters. If name is longer than 63
        // characters are registered using the host (numeric) address instead of the host name.
		// hostname   =  shortname *( "." shortname )
		// hostaddr   =  ip4addr / ip6addr
};

#endif
