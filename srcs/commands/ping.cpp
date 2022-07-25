#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

std::string ping(int fd, std::vector<std::string> params, Server *srv)
{
    // param check ----------------------------------------------------------- /
    // check nb of param
    if (params.size() == 0)
        return (reply(srv, fd, "461", ERR_NEEDMOREPARAMS(std::string("KILL"))));
    if (params.size() != 2)
        return (reply(srv, fd, "461", ERR_NEEDMOREPARAMS(std::string("KILL"))));
    return (NULL);
    
}
