#ifndef MISCELLANEOUS_CMDS_HPP
# define MISCELLANEOUS_CMDS_HPP

# include <vector>
# include <string>

class Server;

// Miscellaneous commands
std::string kill(const int fd, std::vector<std::string> cmds, Server *srv);
std::string ping(const int fd, std::vector<std::string> cmds, Server *srv);
std::string pong(const int fd, std::vector<std::string> cmds, Server *srv);

#endif
