#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

bool areValidServiceParams(const std::vector<std::string> &params) 
{	
	if (forbiddenNick(params[0]) || params[0].find(' ') != std::string::npos)
		return false;
	else if (params[1][0] != '*' || params[1].size() > 1)
		return false;
	else if (params[2].find(HOSTADDRESS) == std::string::npos)
		return false;	
	else if (isNumber(params[3]) && std::atol(params[3].c_str()) > 9)
		return false;
	else if (isNumber(params[4]) && std::atol(params[4].c_str()) > 9)
		return false;
	return true;
}

void service(const int &fd, const std::vector<std::string> &params, const std::string &, 
		Server *srv) 
{
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	if (user != 0 && user->getPassword() == true) {
		if (params.empty() || params.size() < 6 || emptyParams(params)) {
				replyMsg = numericReply(srv, fd, "461",
					ERR_NEEDMOREPARAMS(std::string("SERVICE")));
		}
		else if (srv->getUserByNickname(params[0]) != 0) 
			replyMsg = numericReply(srv, fd, "433", ERR_NICKNAMEINUSE(params[0]));
		else if (!user->getUsername().empty())
			replyMsg = numericReply(srv, fd, "462", ERR_ALREADYREGISTRED);
		else if (areValidServiceParams(params) == true) {
			user->setNickname(params[0]);
			user->setUsername(params[0]);
			user->setFullname(params[5]);
			user->setIsBot(true);
			if (isAuthenticatable(user)) 
				authenticateUser(fd, srv);
			return ;
		}
	}
	if (!replyMsg.empty())
		srv->sendClient(fd, replyMsg);
	return ;
}
