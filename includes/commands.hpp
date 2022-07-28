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
void		pass(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void		nick(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void		user(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Commands to ignore
void cap(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Server queries and commands
void motd(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void time(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void version(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void admin(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void info(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Miscellaneous commands
void join(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void part(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void invite(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void kick(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void kill(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void ping(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void pong(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

#endif
