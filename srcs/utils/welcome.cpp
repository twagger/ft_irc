#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/Server.hpp"

bool 	isAuthenticationCmd(std::string cmd) {
	if (cmd.compare("USER") == 0 || cmd.compare("NICK") == 0
		|| cmd.compare("PASS") == 0 || cmd.compare("QUIT") == 0
		|| cmd.compare("SERVICE") == 0)
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

void addDefaultMode(Server *srv, User *user, const int fd, short mod) {
	std::vector<std::string> params;
	user->addMode(mod);
	params.push_back(user->getNickname());
	mode(fd, params, "MODE", srv);
}

void	authenticateUser(const int fd, Server *srv)
{
	std::string					replyMsg;
	std::vector<std::string>	params;
	User*						user = srv->getUserByFd(fd);
	bool						isBot = user->getIsBot();

	if (!isBot)
		replyMsg.append(numericReply(srv, fd, "001",
			RPL_WELCOME(user->getNickname(), user->getUsername(), user->getHostname())));
	else
		replyMsg.append(numericReply(srv, fd, "383",
			RPL_YOURESERVICE(user->getNickname())));	
	replyMsg.append(numericReply(srv, fd, "002",
		RPL_YOURHOST(srv->getHostname(), srv->getVersion())));
	if (!isBot)
		replyMsg.append(numericReply(srv, fd, "003", RPL_CREATED(srv->getDate())));
	replyMsg.append(numericReply(srv, fd, "004",
		RPL_MYINFO(srv->getHostname(), srv->getVersion(), USERMODES, CHANNELMODES)));
	srv->sendClient(fd, replyMsg);
	if (!isBot) {
		motd(fd, params, "MOTD", srv);
		addDefaultMode(srv, user, fd, MOD_INVISIBLE);
	}
	else
		addDefaultMode(srv, user, fd, MOD_BOT);
	user->setAuthenticated(true);																							// is client answering smth?
	return ;
}
