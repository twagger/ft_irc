#ifndef MISCELLANEOUS_CMDS_HPP
# define MISCELLANEOUS_CMDS_HPP

# include <vector>
# include <string>

class Server;

// Miscellaneous commands
const std::string kill(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string ping(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string pong(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string join(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string part(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string invite(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);
const std::string kick(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv);

#endif
