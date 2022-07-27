#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

const std::string	pass(const int &fd, const std::vector<std::string> &params, const std::string &, Server *srv) {
	
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	if (user != 0)
	{
		if (params.empty() || params[0].empty()) {
			replyMsg = numericReply(srv, fd, "461",
				ERR_NEEDMOREPARAMS(std::string("PASS")));
		}
		else if (user->getPassword() == true) {
			replyMsg = numericReply(srv, fd, "462", ERR_ALREADYREGISTRED); 
		}
		else if (!user->getPassword() && srv->getPassword()
			== params[0]) {	
			user->setPassword(true);
		}
	}
	return replyMsg;
}
