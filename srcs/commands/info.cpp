#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/exceptions.hpp"

/**
 * @brief The INFO command is REQUIRED to return information describing the
 * server
 * 
 * Errors handled:
 * - ERR_NOSUCHSERVER
 *   
 */

std::string createInfoStr(Server *srv, const int &fd) {
    std::string	replyMsg;
    replyMsg
    .append(numericReply(srv, fd, "371", \
            RPL_INFO(std::string("Version: ").append(VERSION))))
    .append(numericReply(srv, fd, "371", \
            RPL_INFO(std::string("Version comment: ").append(VCOMMENT))))
    .append(numericReply(srv, fd, "371", \
            RPL_INFO(std::string("Compilation Date: ").append(COMPILDATE))));
    return (replyMsg);
}

void    info(const int &fd, const std::vector<std::string> &params, \
                       const std::string &,Server *srv)
{
    std::string reply;


    // COMMAND EXECUTION
    try {
        if (params.size() > 0) {
            if (params[0].compare(srv->getHostname()) != 0)
                throw nosuchserverException(params[0]);
        }   
        reply
        .append(createInfoStr(srv, fd))
        .append(numericReply(srv, fd, "374", RPL_ENDOFINFO));
        srv->sendClient(fd, reply);
    }
    // EXCEPTIONS
    catch (nosuchserverException &e) {e.reply(srv, fd); return; }
}
