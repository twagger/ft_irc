#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

// user =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF )
//     ; any octet except NUL, CR, LF, " " and "@"
//	0	00	NUL
//	10	0A	LF
//	13	0D	CR
//	32	20	' '
//	64	40	@
bool	forbiddenUsername(std::string param) 
{
    if (param.empty())
		return true;
	else if (param.find('\0') != std::string::npos)
		return true;
	else if (param.find(10) != std::string::npos)
		return true;
	else if (param.find(13) != std::string::npos)
		return true;
	else if (param.find(64) != std::string::npos)
		return true;
	return false;
}

bool areValidParams(std::vector<std::string> params) 
{	
	for (unsigned int i = 0; i < params.size(); i++) {
		if (params[i].empty()) 	
			return false;
	}
	if (forbiddenUsername(params[0]) || params[0].find(32) != std::string::npos)
		return false;
	else if (isdigit(params[1][0]) == 0 || params[1][0] < '0' || params[1][0] > '7')
		return false;
	else if (params[3].size() < 1 || forbiddenUsername(params[3]))
		return false;
	return true;
}

std::string user(const int fd, std::vector<std::string> params, Server *irc) 
{
	std::string replyMsg;
	User *user = irc->getUserByFd(fd);

	if (user != 0 && user->getPassword() == true) {
		if (params.empty() || params.size() < 4) {
			// should check for empty params here? => to be tested
			replyMsg = numericReply(irc, fd, "461",
				ERR_NEEDMOREPARAMS(std::string("USER")));
		}
		else if (!user->getUsername().empty()) {
			replyMsg = numericReply(irc, fd, "462", ERR_ALREADYREGISTRED); 
		}
		else if (areValidParams(params) == true) {
			user->setUsername(params[0]);
			user->setMode(params[1][0]);
			user->setFullname(params[3]);
			if (isAuthenticatable(user)) 
				replyMsg = authenticateUser(fd, irc);
		}
	}
	return (replyMsg);
}
