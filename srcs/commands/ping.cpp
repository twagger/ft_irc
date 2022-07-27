#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

const std::string ping(const int &fd, const std::vector<std::string> &params, \
                       const std::string &, Server *srv)
{
    std::string hostname;

    // check nb of param
    if (params.size() == 1) // only hostname is expected
    {
        hostname = params[0];
        // check if hostname is the one of the server
        if (srv->getHostname() != hostname)
            return (numericReply(srv, fd, "402", ERR_NOSUCHSERVER(hostname)));
        // answer with a pong
        return (PONG(hostname));
    }
    return (std::string()); 
}
