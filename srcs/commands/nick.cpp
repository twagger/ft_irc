#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

bool	forbiddenNick(std::string param)
{
    if (param.empty())
		return true;
	else if (!isalpha(param[0]) && std::string(NAMESPECIALS).find(param[0])
				== std::string::npos)										
		return true;
	else if (param.length() > 9)												// to be check if > 9 >> erroneous nickname msg
		return true;
	else {
		for (unsigned int i = 0; i < param.length(); i++) {
			if (!isalnum(param[i]) && std::string(NAMESPECIALS).find(param[i])
				== std::string::npos && param[i] != 45)
				return true;
		}
	}
	return false;
}

bool isInKillList(Server *srv, std::string nick) {
	
	std::map<std::string, time_t>::iterator it = srv->_unavailableNicknames.find(nick);
	std::map<std::string, time_t>::iterator ite = srv->_unavailableNicknames.end();

	if (it != ite) {
		if (time(NULL) - (srv->_unavailableNicknames.find(nick))->second < KILLTIME)
			return (true);
		else {
			srv->_unavailableNicknames.erase(it);
			return (false);			
		}
	}
	return (false);
}

const std::string nick(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv) 
{	
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);
	if (user != 0 && user->getPassword() == true)
	{
		if (params.empty() || params[0].empty()) {
			replyMsg = numericReply(srv, fd, "431", ERR_NONICKNAMEGIVEN);
		}
		else if (forbiddenNick(params[0]) == true) {
			replyMsg = numericReply(srv, fd, "432", ERR_ERRONEUSNICKNAME(params[0]));
		}
		else if (srv->getUserByNickname(params[0]) != 0) {
			replyMsg = numericReply(srv, fd, "433", ERR_NICKNAMEINUSE(params[0]));
		}
		else if (isInKillList(srv, params[0])) {
			replyMsg = numericReply(srv, fd, "437", ERR_UNAVAILRESOURCE(params[0]));
		}
		else if (user->hasMode(MOD_RESTRICTED)) {
			replyMsg = numericReply(srv, fd, "484", ERR_RESTRICTED);
		}
		else if (user->getNickname() == "*") {
			user->setNickname(params[0]);
			if (isAuthenticatable(user)) 
				replyMsg = authenticateUser(fd, srv);
		}
		else {
			replyMsg = clientReply(srv, fd, CLIENT_NICK(prefix, params[0]));			
			user->setNickname(params[0]);
		}
	}
	return replyMsg;
}
