#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/replies.hpp"

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

const std::string info(const int &fd, const std::vector<std::string> &, \
                       const std::string &,Server *srv)
{
    std::string reply;
    
    reply
    .append(createInfoStr(srv, fd))
    .append(numericReply(srv, fd, "374", RPL_ENDOFINFO));
    return (reply);
}
