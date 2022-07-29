#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

bool 	isAuthenticationCmd(std::string cmd) {
	if (cmd.compare("USER") == 0 || cmd.compare("NICK") == 0
		|| cmd.compare("PASS") == 0)
		return true;
	return false;
}

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

void addModeI(Server *srv, User *user, const int fd) {
	std::vector<std::string> params;
	params.push_back(user->getNickname());
	params.push_back("+i");
	std::cout << "[DEBUG] test add mode in welcome" << std::endl;
	mode(fd, params, "MODE", srv);
}

void	authenticateUser(const int fd, Server *srv)
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
	srv->sendClient(fd, replyMsg);
	motd(fd, params, "MOTD", srv);
	addModeI(srv, user, fd);
	user->setAuthenticated(true);																							// is client answering smth?
	return ;
}
