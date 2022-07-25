#ifndef MISCELLANEOUS_CMDS_HPP
# define MISCELLANEOUS_CMDS_HPP

# include <vector>
# include <string>
# include <iostream>
# include <sys/socket.h>
# include <typeinfo>

class Server;
class User;

// Auth commands
std::string	pass(int fd, std::vector<std::string> params, Server *irc);
bool		forbiddenNick(std::string param);
std::string nick(const int fd, std::vector<std::string> params, Server *irc);
bool		forbiddenUsername(std::string param);
std::string user(const int fd, std::vector<std::string> params, Server *irc);
bool		isAuthenticatable(User *user);
std::string	authenticateUser(const int fd, Server *irc);

// Miscellaneous commands
std::string kill(const int fd, std::vector<std::string> cmds, Server *srv);
std::string ping(const int fd, std::vector<std::string> cmds, Server *srv);
std::string pong(const int fd, std::vector<std::string> cmds, Server *srv);
std::string join(const int fd, std::vector<std::string> cmds, Server *srv);

#endif
