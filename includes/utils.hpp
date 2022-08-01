#ifndef UTILS_HPP
# define UTILS_HPP

# include "Server.hpp"

# include <string>
# include <vector>
# include "replies.hpp"
# include "Server.hpp"

// Util functions
int                         printError(std::string message, int code, \
                                        bool with_errno);

// parsing
std::vector<std::string> splitBy(std::string str, const std::string &delimiter);
const std::vector<Command>     splitCmds(std::vector<std::string> cmd_strings);

// Util functions
std::string numericReply(Server *irc, const int &fd, std::string code,
			std::string replyMsg);
std::string	clientReply(Server *irc, const int &fd, std::string replyMsg);
std::string WelcomeChan(Server *irc, const int &fd, std::string code,
    std::map<std::string, Channel *>::iterator itMap,
    std::string channelName);
std::string eventChannel(Server *irc, const int &fd, std::string eventType,
    std::string channelName);
int         print_error(std::string message, int code, bool with_errno);
std::string get_next_tokn(std::string *str, std::string delimiter);

// Channel util functions
std::vector<std::string> splitByComma(std::string parameter);
bool                     isChannel(std::string channel_name);
std::string              getChannelTopic(std::string channelName, std::map<std::string,
    Channel *> channelList);
std::vector<std::string> getChannelKey(std::vector<std::string> parameter);
std::deque<User *>::iterator findUserOnChannel(std::deque<User *> userList, User *currentUser);
std::vector<char>::iterator findMode(std::vector<char> listMode, char mode);

#endif
