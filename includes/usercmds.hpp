#ifndef USERCMDS_HPP
# define USERCMDS_HPP

# include <string>
# include <iostream>
# include <vector>
class Server;

std::string	pass(int fd, std::vector<std::string> params, Server *irc);


#endif