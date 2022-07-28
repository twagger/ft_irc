#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"



void	oper(const int &fd, const std::vector<std::string> &params, const std::string &prefix, 
		Server *srv) {

	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	if (params.empty() || params.size() < 2 || emptyParams(params)) {
		replyMsg = numericReply(srv, fd, "461", ERR_NEEDMOREPARAMS(prefix));
	}
	// else if (isOperHost(user->getHostname()))
	// 	replyMsg = numericReply(srv, fd, "491", ERR_NOOPERHOST);
	else if (params[1] != srv->getPassword())
		replyMsg = numericReply(srv, fd, "464", ERR_PASSWDMISMATCH);
	else {
		user->addMode(MOD_OPER);
		mode(fd, user->getNickname(), "MODE", srv);
	}
}