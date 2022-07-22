#ifndef MISCELLANEOUS_CMDS_HPP
# define MISCELLANEOUS_CMDS_HPP

# include <vector>
# include <string>

class Server;

// Miscellaneous commands
std::string kill(int fd, std::vector<std::string> cmds, Server *srv);
std::string ping(int fd, std::vector<std::string> cmds, Server *srv);
std::string pong(int fd, std::vector<std::string> cmds, Server *srv);

#endif
