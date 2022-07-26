#ifndef USERCMDS_HPP
# define USERCMDS_HPP

# include <string>
# include <iostream>
# include <vector>
# include <sys/socket.h>
# include <typeinfo>

class Server;
class User;

const std::string	pass(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *irc);

bool	forbiddenNick(std::string param);
const std::string nick(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *irc);

bool	forbiddenUsername(std::string param);

bool	isAuthentifiable(User *user);
std::string authenticateUser(const int fd, Server *irc, std::string replyMsg);


#endif
