#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

std::string pong(int fd, std::vector<std::string> params, Server *srv)
{
    std::string clientname;
    User        *user;

    // check nb of param
    if (params.size() == 1) // only clientname is expected
    {
        clientname = params[0];
        // update client status ---------------------------------------------- /
        if ((user = srv->getUserByFd(fd)) == NULL)
        {
            printError("Unknown user", 1, false);
            return (NULL);
        }
        user->setStatus(ST_ALIVE);
    }
    return (NULL);
}
