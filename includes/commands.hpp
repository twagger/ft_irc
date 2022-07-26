#ifndef MISCELLANEOUS_CMDS_HPP
# define MISCELLANEOUS_CMDS_HPP

# include <vector>
# include <string>

class Server;

// Commands to ignore
const std::string cap(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Server queries and commands
const std::string motd(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string time(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string version(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string admin(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string info(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

// Miscellaneous commands
const std::string kill(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string ping(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string pong(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string join(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

#endif
