#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/Server.hpp"

void	quit(const int &fd, const std::vector<std::string> &params, const std::string &, Server *srv) 
{
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	// Send ERROR to the user encapsulated in NOTICE  ----------------------------- /
	serverQuitNotice(fd, srv, user->getNickname(), 
		params.empty() == true ? CLIENT_ERROR : CLIENT_ERRORMSG(params[0]));

	// Make the replyMsg and get channels from the user before killing the fd ----- /
	replyMsg = clientReply(srv, fd, CLIENT_QUIT(std::string("QUIT"),
		(params.empty() == true ? std::string() : params[0])));
	std::deque<std::string> channelsToReplyTo = user->getChannelsJoined();
	std::deque<std::string>::iterator it;

	// Kill the client fd -------------------------------------------------------- /
	try { srv->killConnection(fd); }
	catch (Server::pollDelException &e) 
		{ printError(e.what(), 1, true); }
	catch (Server::invalidFdException &e) 
		{ printError(e.what(), 1, false); }
	
	// Send a client reply from origin FD to all channels where the user was ----- /
	try {
		for (it = channelsToReplyTo.begin(); it < channelsToReplyTo.end(); ++it) {
			srv->sendChannel(*it, replyMsg);
		}
	}
	catch (Server::invalidChannelException &e) 
	{ printError(e.what(), 1, true); }

	return ;
}
