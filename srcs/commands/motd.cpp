#include "../../includes/commands.hpp"
#ifndef MOTD
# define MOTD ""
#endif

// RPL_MOTDSTART(server)
// RPL_MOTD(text)
// RPL_ENDOFMOTD

const std::string motd(const int &fd, const std::vector<std::string> &, \
                       const std::string &,Server *srv)
{
    size_t      len;
    std::string motd;

    // ignore the params and send MOTD to requester fd
    if (!MOTD.empty())
    {
        len = MOTD.length();
        srv->sendClient(fd, )
        // cut by packet of 510 and send
    }
    else
        return (numericReply(srv, fd, "422", ERR_NOMOTD))
    
    return (NULL);
}
