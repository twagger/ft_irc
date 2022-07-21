#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

std::string kill(int fd, std::vector<std::string> cmds, Server *srv)
{
    // param check
   return(reply(srv, fd, "401", ERR_NOSUCHNICK(cmds[0]))); 
}
