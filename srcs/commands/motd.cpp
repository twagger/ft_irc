#include <deque>

#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

#ifndef MOTD
# define MOTD ""
#endif

// RPL_MOTDSTART(server)
// RPL_MOTD(text)
// RPL_ENDOFMOTD

// Util function : cut a string in multiple 510 char packets in a deque
const std::deque<std::string>   splitByParts(std::string str, size_t partSize)
{
    std::deque<std::string> parts;
    size_t                  len;

    len = str.length();
    while (len % partSize > 0)
    {
        parts.push_back(str.substr(0, partSize));
        str.erase(0, partSize);
        len -= partSize;
    }
    if (!str.empty())
        parts.push_back(str);
    return (parts);
}

const std::string motd(const int &fd, const std::vector<std::string> &, \
                       const std::string &,Server *srv)
{
    size_t                                  len;
    std::string                             motd = MOTD;
    std::string                             tmpString;
    std::deque<std::string>                 parts;
    std::deque<std::string>::const_iterator it;

    // ignore the params and send MOTD to requester fd
    if (!motd.empty())
    {
        // start
        srv->sendClient(fd, numericReply(srv, fd, "375", \
                        RPL_MOTDSTART(srv->getHostname())));
        // split motd in multiple 510 char parts
        parts = splitByParts(motd, 510);
        for (it = parts.begin(); it != parts.end(); ++it)
        {
            srv->sendClient(fd, numericReply(srv, fd, "372", RPL_MOTD(*it)));
        }
        // stop
        srv->sendClient(fd, numericReply(srv, fd, "376", RPL_ENDOFMOTD));
        return (NULL);
    }
    else
        return (numericReply(srv, fd, "422", ERR_NOMOTD));
}
