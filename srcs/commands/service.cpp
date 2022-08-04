#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"


//       Command: SERVICE
//    Parameters: <nickname> <reserved> <distribution> <type>
//                <reserved> <info>

//    The SERVICE command to register a new service.  Command parameters
//    specify the service nickname, distribution, type and info of a new
//    service.
//    The <distribution> parameter is used to specify the visibility of a
//    service.  The service may only be known to servers which have a name
//    matching the distribution.  For a matching server to have knowledge
//    of the service, the network path between that server and the server
//    on which the service is connected MUST be composed of servers which
//    names all match the mask.

//    The <type> parameter is currently reserved for future usage.

//    Numeric Replies:

//            ERR_ALREADYREGISTRED            ERR_NEEDMOREPARAMS
//            ERR_ERRONEUSNICKNAME
//            RPL_YOURESERVICE                RPL_YOURHOST
//            RPL_MYINFO

//    Example:

//    SERVICE dict * *.fr 0 0 :French Dictionary ; Service registering
//                                    itself with a name of "dict".  This
//                                    service will only be available on
//                                    servers which name matches "*.fr".

bool areValidServiceParams(const std::vector<std::string> &params, std::string srvHostname) 
{	

	if (forbiddenNick(params[0]) || params[0].find(' ') != std::string::npos)
		return false;
	else if (params[1][0] != '*' || params[1].size() > 1)
		return false;
	else if (params[2] != srvHostname)
		return false;	
	else if (isNumber(params[3]) && std::atol(params[3].c_str()) > 9)
		return false;
	else if (isNumber(params[4]) && std::atol(params[4].c_str()) > 9)
		return false;
	else if (params[5][0] != ':')
		return false;
	return true;
}

void service(const int &fd, const std::vector<std::string> &params, const std::string &, 
		Server *srv) 
{
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	if (user != 0 && user->getPassword() == true) {
		if (user != 0 && user->getPassword() == true) {
			if (params.empty() || params.size() < 6 || emptyParams(params)) {
				replyMsg = numericReply(srv, fd, "461",
					ERR_NEEDMOREPARAMS(std::string("SERVICE")));
			}
		}
		else if (!user->getUsername().empty()) {
			replyMsg = numericReply(srv, fd, "462", ERR_ALREADYREGISTRED); 
		}
		else if (areValidServiceParams(params, srv->getHostname()) == true) {
			user->setNickname(params[0]);
			user->setUsername(params[0]);
			user->addMode(MOD_BOT);
			user->setFullname(params[5]);
			if (isAuthenticatable(user)) 
				authenticateUser(fd, srv);
			return ;
		}
	}
	if (!replyMsg.empty())
		srv->sendClient(fd, replyMsg);
	return ;
}