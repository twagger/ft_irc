#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <vector>
# include <string>
# include <iostream>
# include <sys/socket.h>
# include <typeinfo>

class Server;
class User;

// Connexion commands
void	pass(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void	nick(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void	user(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void 	mode(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void	oper(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv); 
void	quit(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv); 

// Server queries and commands
void	motd(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void	time(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void	version(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void	admin(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void	info(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Commands to ignore
void	cap(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Channel command
void    join(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void    part(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void    invite(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void    kick(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void    topic(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void    list(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void    names(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Miscellaneous commands
void    kill(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void    ping(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void    pong(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void 	die(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv); 

// Sending message commands
void    privmsg(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void    notice(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// User based queries
void    who(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
void    whois(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Services commands
void service(const int &fd, const std::vector<std::string> &params, const std::string &, 
		Server *srv);

#endif
