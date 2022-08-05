#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"

/**
 * @brief PONG message is a reply to ping message. If it is sent by client to 
 * the server, then the server updates the client status to mark it as alive.
 * 
 * Errors handled:
 * - ERR_NOORIGIN
 * - ERR_NOSUCHSERVER
 *   
 */

void pong(const int &fd, const std::vector<std::string> &params, \
                       const std::string &, Server *srv)
{
    std::string origin;
    std::string servername;
    User        *user;

    // COMMAND EXECUTION
    try {
        if (params.size() == 0)
            throw nooriginException();
        if (params.size() >= 1)
        {
            origin = params[0];
            if (params.size() == 2)
            {
                // In mono-server, the target server must be the current one
                servername = params[1];
                if (srv->getHostname().compare(servername) != 0)
                    throw nosuchserverException(servername);
            }

            // update client status with the fd, as we are not in multiserv
            if ((user = srv->getUserByFd(fd)) != NULL)
                user->setStatus(ST_ALIVE);           
        }
    }

    // EXCEPTIONS
    catch (nooriginException &e) {e.reply(srv, fd); return; }
    catch (nosuchserverException &e) {e.reply(srv, fd); return; }
}
