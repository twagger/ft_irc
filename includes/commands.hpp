#ifndef MISCELLANEOUS_CMDS_HPP
# define MISCELLANEOUS_CMDS_HPP

# include <vector>
# include <string>
# include <iostream>
# include <sys/socket.h>
# include <typeinfo>

class Server;
class User;

// Connexion commands
const std::string	pass(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
bool		forbiddenNick(std::string param);
const std::string nick(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
bool		forbiddenUsername(std::string param);
const std::string user(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
bool		isAuthenticatable(User *user);
std::string	authenticateUser(const int fd, Server *srv);
const std::string quit(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv); 


// Miscellaneous commands
const std::string kill(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string ping(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string pong(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string join(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

#endif
