#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

void	serverNotice(const int &fd,  Server *srv, const std::string &destNick, std::string msg) {
	
	std::string reply = ":" + srv->getHostname() + " NOTICE " + destNick + " " + ":" + msg;
	try { srv->sendClient(fd, reply); }
	catch (Server::sendException &e)
	{ printError(e.what(), 1, true); return; }
}

const std::string quit(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv) 
{
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	// Send ERROR to the user encapsulated in NOTICE
	if (params.empty() || params[0].empty())
		serverNotice(fd, srv, user->getNickname(), CLIENT_ERROR);
	else
		serverNotice(fd, srv, user->getNickname(), CLIENT_ERRORMSG(params[0]));	

	// // Send a client reply from origin FD to all channels where the user was present
	std::vector<std::string> channelsToReplyTo = user->getChannelsJoined();
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = channelsToReplyTo.end();
	replyMsg = clientReply(srv, fd, CLIENT_QUIT(prefix, params[0]));
	for (it = channelsToReplyTo.begin(); it < ite; ++it) {
		srv->sendChannel(*it, replyMsg);
	}

	//Try to kill the fd (should be done after sending reply to all channels?)
	try { srv->killConnection(fd); }
    catch (Server::pollDelException &e) { printError(e.what(), 1, true); }
    catch (Server::invalidFdException &e) { printError(e.what(), 1, false); }
	return (std::string());
}
