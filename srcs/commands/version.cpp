#include "../../includes/commands.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"

void version(const int &fd, \
        const std::vector<std::string> &, const std::string &,Server *srv)
{
    srv->sendClient(fd, numericReply(srv, fd, "351", \
        RPL_VERSION(std::string(VERSION), std::string(""), srv->getHostname(), \
                    std::string(VCOMMENT))));
}
