#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

void	serverNotice(const int &fd,  Server *srv, const std::string &destNick, std::string msg) {
	
	std::string reply = ":" + srv->getHostname() + " NOTICE " + destNick + " " + ":" + msg;
	srv->sendClient(fd, reply);
}

void	quit(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv) 
{
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	// Send ERROR to the user encapsulated in NOTICE
	if (params.empty() || params[0].empty())
		serverNotice(fd, srv, user->getNickname(), CLIENT_ERROR);
	else
		serverNotice(fd, srv, user->getNickname(), CLIENT_ERRORMSG(params[0]));	

	// // Send a client reply from origin FD to all channels where the user was present
	std::deque<std::string> channelsToReplyTo = user->getChannelsJoined();
	std::deque<std::string>::iterator it;
	std::deque<std::string>::iterator ite = channelsToReplyTo.end();
	replyMsg = clientReply(srv, fd, CLIENT_QUIT(prefix, params[0]));
	for (it = channelsToReplyTo.begin(); it < ite; ++it) {
		srv->sendChannel(*it, replyMsg);
	}

	//Try to kill the fd (should be done after sending reply to all channels?)
	try { srv->killConnection(fd); }
    catch (Server::pollDelException &e) { printError(e.what(), 1, true); }
    catch (Server::invalidFdException &e) { printError(e.what(), 1, false); }
	return ;
}
