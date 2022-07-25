#ifndef USERCMDS_HPP
# define USERCMDS_HPP

# include <string>
# include <iostream>
# include <vector>
# include <sys/socket.h>
# include <typeinfo>

class Server;
class User;

std::string	pass(int fd, std::vector<std::string> params, Server *irc);

bool	forbiddenNick(std::string param);
std::string nick(const int fd, std::vector<std::string> params, Server *irc);

bool	forbiddenUsername(std::string param);

bool	isAuthentifiable(User *user);
std::string authenticateUser(const int fd, Server *irc, std::string replyMsg);


#endif