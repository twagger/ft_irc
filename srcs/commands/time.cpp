#include "../../includes/commands.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"

void time(const int &fd, \
        const std::vector<std::string> &, const std::string &,Server *srv)
{
    time_t      rawtime = time(NULL);
    struct tm   *timeinfo;
    std::string strTime;

    timeinfo = localtime(&rawtime);
    strTime = std::string(asctime(timeinfo));
    srv->sendClient(fd, \
        numericReply(srv, fd, "391", RPL_TIME(srv->getHostname(), strTime)));
}
