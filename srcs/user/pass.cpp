#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

std::string	pass(const int fd, std::vector<std::string> params, Server *irc) {
	
	std::string replyMsg;
	User *user = irc->getUserByFd(fd);

	if (user != 0)
	{
		if (params.empty() || params[0].empty()) {
			replyMsg = numericReply(irc, fd, "461",
				ERR_NEEDMOREPARAMS(std::string("PASS")));
		}
		else if (user->getPassword() == true) {
			replyMsg = numericReply(irc, fd, "462", ERR_ALREADYREGISTRED); 
		}
		else if (!user->getPassword() && irc->getPassword()
			== params[0]) {	
			user->setPassword(true);
		}
	}
	return replyMsg;
}
