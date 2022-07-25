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

std::string	authenticateUser(const int fd, Server *irc)
{
	std::string	replyMsg;
	User		*user = irc->getUserByFd(fd);

	replyMsg.append(numericReply(irc, fd, "001",
		RPL_WELCOME(user->getNickname(), user->getUsername(), user->getHostname())));
	replyMsg.append(numericReply(irc, fd, "002",
		RPL_YOURHOST(irc->getHostname(), irc->getVersion())));
	replyMsg.append(numericReply(irc, fd, "003",
		RPL_CREATED(irc->getDate())));
	replyMsg.append(numericReply(irc, fd, "004",
		RPL_MYINFO(irc->getHostname(), irc->getVersion(), USERMODES, CHANNELMODES)));
//	replyMsg.append(modt());
	user->setAuthenticated(true);																								// is client answering smth?
	return (replyMsg);
}
