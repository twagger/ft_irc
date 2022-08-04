#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"

/**
 * @brief The PING command is used to test the presence of an active client or
 * server at the other end of the connection.
 * 
 * Errors handled:
 * - ERR_NOORIGIN
 * - ERR_NOSUCHSERVER
 *   
 */

void    ping(const int &fd, const std::vector<std::string> &params, \
                       const std::string &, Server *srv)
{
    std::string origin;
    std::string servername;

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
            // answer with a pong, use fd instead of origin as we are not in
            // multiserv and PING queries cannot be forwarded
            srv->sendClient(fd, PONG(srv->getHostname()));
        }
    }

    // EXCEPTIONS
    catch (nooriginException &e) {e.reply(srv, fd); return; }
    catch (nosuchserverException &e) {e.reply(srv, fd); return; }
}
