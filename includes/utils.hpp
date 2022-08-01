#ifndef UTILS_HPP
# define UTILS_HPP

# include "Server.hpp"
# include "replies.hpp"

# include <string>
# include <vector>
# include <stdlib.h>
# include <sstream>
# include <fstream>
# include <string>
# include <cstring>
# include <algorithm>
# include <sys/socket.h>

// Util functions
int                         printError(std::string message, int code,
	bool with_errno);
bool emptyParams(const std::vector<std::string> &params);

// Parsing
std::vector<std::string> splitBy(std::string str, const std::string &delimiter);
const std::vector<Command>     splitCmds(std::vector<std::string> cmd_strings);

// Replies util functions
std::string numericReply(Server *irc, const int &fd, std::string code,
			std::string replyMsg);
std::string	clientReply(Server *irc, const int &fd, std::string replyMsg);
std::string WelcomeChan(Server *irc, const int &fd, std::string code,
    std::map<std::string, Channel *>::iterator itMap,
    std::string channelName);
std::string eventChannel(Server *irc, const int &fd, std::string eventType,
    std::string channelName);
//std::string get_next_tokn(std::string *str, std::string delimiter);
//int         print_error(std::string message, int code, bool with_errno);

// Channel util functions
std::vector<std::string> splitByComma(std::string parameter);
bool                     isChannel(std::string channel_name);
std::string              getChannelTopic(std::string channelName, std::map<std::string,
    Channel *> channelList);
std::vector<std::string> getChannelKey(std::vector<std::string> parameter);
std::deque<User *>::iterator findUserOnChannel(std::deque<User *> userList,
	User *currentUser);
std::vector<char>::iterator findMode(std::vector<char> listMode, char mode);

// Authenticate users
bool 		isAuthenticationCmd(std::string cmd);
bool		isAuthenticatable(User *user);
void		authenticateUser(const int fd, Server *srv);

#endif
