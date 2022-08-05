#include "../../includes/commands.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"

/**
 * @brief The VERSION command is used to query the version of the server
 * program.  An optional parameter <target> is used to query the version
 * of the server program which a client is not directly connected to.
 * 
 * Errors handled:
 * - ERR_NOSUCHSERVER
 *   
 */

void version(const int &fd, \
        const std::vector<std::string> &params, const std::string &,Server *srv)
{
    // COMMAND EXECUTION
    try {
        if (params.size() > 0) {
            if (params[0].compare(srv->getHostname()) != 0)
                throw nosuchserverException(params[0]);
        }   
        srv->sendClient(fd, numericReply(srv, fd, "351", \
        RPL_VERSION(std::string(VERSION), std::string(""), srv->getHostname(), \
                    std::string(VCOMMENT))));
    }
    // EXCEPTIONS
    catch (nosuchserverException &e) {e.reply(srv, fd); return; }
}
