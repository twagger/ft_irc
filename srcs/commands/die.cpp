#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/replies.hpp"

void die(const int &fd, const std::vector<std::string> &, const std::string &,
			Server *srv) 
{	
	if (srv->getUserByFd(fd)->hasMode(MOD_OPER)) {
		informSUsers(srv, "Server is shutting down");
		throw Server::pollWaitException();
	}
	else 
		return (srv->sendClient(fd, numericReply(srv, fd, "481",
												ERR_NOPRIVILEGES)));
}
