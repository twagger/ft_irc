#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
//  1 =>  A-Z / a-z ";" "[", "]", "\", "`", "_", "^", "{", "|", "}"
//  8 => A-Z / a-z 0/9 ";" "[", "]", "\", "`", "_", "^", "{", "|", "}", "-"

bool	forbiddenNick(std::string param)
{
    if (param.empty())
		return true;
	else if (!isalpha(param[0]) && std::string(NAMESPECIALS).find(param[0])
				== std::string::npos)										
				//&& param[0] != 92 ?
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
		// else if (// nick is in kill list)											// waiting for killlist
		// 	replyMsg = numericReply(srv, fd, "437", ERR_UNAVAILRESOURCE(params[0]));
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
