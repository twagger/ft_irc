#include "../../includes/commands.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"

/**
 * @brief The time command is used to query local time from the specified
 * server. If the <target> parameter is not given, the server receiving
 * the command must reply to the query.
 * 
 * Errors handled:
 * - ERR_NOSUCHSERVER
 *   
 */

void time(const int &fd, \
        const std::vector<std::string> &params, const std::string &,Server *srv)
{
    time_t      rawtime = time(NULL);
    struct tm   *timeinfo;
    std::string strTime;

    // COMMAND EXECUTION
    try {
        if (params.size() > 0) {
            if (params[0].compare(srv->getHostname()) != 0)
                throw nosuchserverException(params[0]);
        }
        timeinfo = localtime(&rawtime);
        strTime = std::string(asctime(timeinfo));
        strTime = strTime.substr(0, strTime.length() - 1);
        srv->sendClient(fd, \
           numericReply(srv, fd, "391", RPL_TIME(srv->getHostname(), strTime)));
    }
    // EXCEPTIONS
    catch (nosuchserverException &e) {e.reply(srv, fd); return; }
}
