#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/Server.hpp"

// user =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF )
//     ; any octet except NUL, CR, LF, " " and "@"

bool	forbiddenUsername(std::string param) 
{
    if (param.empty())
		return true;
	else if (param.find('\0') != std::string::npos)
		return true;
	else if (param.find('\n') != std::string::npos)
		return true;
	else if (param.find('\r') != std::string::npos)
		return true;
	else if (param.find('@') != std::string::npos)
		return true;
	return false;
}

bool isNumber(std::string num) {
	for (unsigned int i = 0; i < num.size(); i++) {
		if (isdigit(num[i]) == 0)
			return false;
	}
	return true;
}

bool areValidParams(const std::vector<std::string> &params) 
{
	if (forbiddenUsername(params[0]) || params[0].find(' ') != std::string::npos)
		return false;
	else if (isNumber(params[1]) && std::atol(params[1].c_str()) > 256)
		return false;
	else if (isNumber(params[1]) == false && (params[1] != params[0]))
		return false;
	else if (params[3].size() < 1 || forbiddenUsername(params[3]))
		return false;
	return true;
}

void user(const int &fd, const std::vector<std::string> &params, const std::string &, 
		Server *srv) 
{
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	if (user != 0 && user->getPassword() == true) {
		if (params.empty() || params.size() < 4 || emptyParams(params)) {
			replyMsg = numericReply(srv, fd, "461",
				ERR_NEEDMOREPARAMS(std::string("USER")));
		}
		else if (!user->getUsername().empty()) {
			replyMsg = numericReply(srv, fd, "462", ERR_ALREADYREGISTRED); 
		}
		else if (areValidParams(params) == true) {
			user->setUsername(params[0]);
			if (params[1] != params[0])
				user->addMode(std::atol(params[1].c_str()));
			user->setFullname(params[3]);
			if (isAuthenticatable(user)) 
				authenticateUser(fd, srv);
			return ;
		}
	}
	if (!replyMsg.empty())
		srv->sendClient(fd, replyMsg);
	return ;
}
