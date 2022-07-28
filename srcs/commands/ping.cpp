#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

void    ping(const int &fd, const std::vector<std::string> &params, \
                       const std::string &, Server *srv)
{
    std::string hostname;

    // check nb of param
    if (params.size() == 1) // only hostname is expected
    {
        hostname = params[0];
        // check if hostname is the one of the server
        if (srv->getHostname() != hostname)
        {
            srv->sendClient(fd, \
                numericReply(srv, fd, "402", ERR_NOSUCHSERVER(hostname)));
            return;
        }
        // answer with a pong
        srv->sendClient(fd, PONG(hostname));
    }
}
