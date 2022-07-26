#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

const std::string quit(const int &fd, const std::vector<std::string> &params, const std::string &prefix, Server *srv) 
{
	std::string replyMsg;


	try { srv->killConnection(fd); }
    catch (Server::pollDelException &e) { printError(e.what(), 1, true); }
    catch (Server::invalidFdException &e) { printError(e.what(), 1, false); }
	if (!params.empty()) {
		replyMsg = clientReply(srv, fd, CLIENT_QUIT(prefix, params[0]));			
	}
	return replyMsg;
}
