#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

bool	isAuthenticatable(User *user) 
{
	if (user->getAuthenticated() == true)
		return false;
	else if (user->getPassword() == false)
		return false;
	else if (user->getNickname() == "*" || user->getUsername().empty())
		return false;
	return true;
}

std::string	authenticateUser(const int fd, Server *srv)
{
	std::string					replyMsg;
	std::vector<std::string>	params;
	User*						user = srv->getUserByFd(fd);

	replyMsg.append(numericReply(srv, fd, "001",
		RPL_WELCOME(user->getNickname(), user->getUsername(), user->getHostname())));
	replyMsg.append(numericReply(srv, fd, "002",
		RPL_YOURHOST(srv->getHostname(), srv->getVersion())));
	replyMsg.append(numericReply(srv, fd, "003",
		RPL_CREATED(srv->getDate())));
	replyMsg.append(numericReply(srv, fd, "004",
		RPL_MYINFO(srv->getHostname(), srv->getVersion(), USERMODES, CHANNELMODES)));
	replyMsg.append(motd(fd, params, "MOTD", srv));
	// user mode +i
	user->setAuthenticated(true);																								// is client answering smth?
	return (replyMsg);
}
