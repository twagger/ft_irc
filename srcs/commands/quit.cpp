#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

void	serverNotice(const int &fd,  Server *srv, const std::string &destNick, std::string msg) {
	
	std::string reply = ":" + srv->getHostname() + " NOTICE " + destNick + " " + ":" + msg;
	srv->sendClient(fd, reply);
}

void	quit(const int &fd, const std::vector<std::string> &params, const std::string &, Server *srv) 
{
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	// Send ERROR to the user encapsulated in NOTICE
	serverNotice(fd, srv, user->getNickname(), 
		params[0].empty() == true ? CLIENT_ERROR : CLIENT_ERRORMSG(params[0]));

	// // Send a client reply from origin FD to all channels where the user was present
	std::deque<std::string> channelsToReplyTo = user->getChannelsJoined();
	std::deque<std::string>::iterator it;

	replyMsg = clientReply(srv, fd, CLIENT_QUIT(std::string("QUIT"),
		(params[0].empty() == true ? std::string() : params[0])));
	try {
		for (it = channelsToReplyTo.begin(); it < channelsToReplyTo.end(); ++it) {
			srv->sendChannel(*it, replyMsg);
		}
	}
	catch (Server::invalidChannelException &e) 
	{ printError(e.what(), 1, true); }

	//Try to kill the fd (should be done after sending reply to all channels?)
	try { srv->killConnection(fd); }
    catch (Server::pollDelException &e) 
	{ printError(e.what(), 1, true); }
    catch (Server::invalidFdException &e) 
	{ printError(e.what(), 1, false); }
	return ;
}

// test segfault on second user quitting a channel
// protect if channel is deleted when all users left 
// broken pipe if /QUIT inside a channel on IRSSI
