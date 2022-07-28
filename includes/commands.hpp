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
const std::string	pass(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
bool		forbiddenNick(std::string param);
const std::string nick(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
bool		forbiddenUsername(std::string param);
const std::string user(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
bool		isAuthenticatable(User *user);
std::string	authenticateUser(const int fd, Server *srv);

// Commands to ignore
void cap(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Server queries and commands
void motd(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void time(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void version(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void admin(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void info(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Miscellaneous commands
void kill(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void ping(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void pong(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string join(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string part(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string invite(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string kick(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

#endif
