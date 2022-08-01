#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

bool isOperHost(std::string hostname) {
	std::string		configFile = OPERCONF;
	const char*			file;
	std::ifstream	input;
	std::string		str;

	file = configFile.c_str();
	input.open(file, std::ios::in);
	try {input.is_open(); } 
	catch (std::ifstream::failure e)
		{ printError(e.what(), 1, true); return false; }
	try {
		while (getline(input, str)) {
			std::cout << "[DEBUG] " << str << " and hostname " << hostname << std::endl;
			if (str == hostname)
				return true;
		}
	}
	catch (std::istream::failure &e) 
    	{ printError(e.what(), 1, true); return false; }
	return false;
}

void	oper(const int &fd, const std::vector<std::string> &params, const std::string &, 
		Server *srv) {

	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	if (params.empty() || params.size() < 2 || emptyParams(params))
		replyMsg = numericReply(srv, fd, "461", ERR_NEEDMOREPARAMS(std::string("OPER")));
	else if (isOperHost(user->getHostname()) == false)
		replyMsg = numericReply(srv, fd, "491", ERR_NOOPERHOST);
	else if (params[1] != srv->getPassword())
		replyMsg = numericReply(srv, fd, "464", ERR_PASSWDMISMATCH);
	else {
		std::vector<std::string> nickname;
		nickname.push_back(user->getNickname());
		user->addMode(MOD_OPER);
		mode(fd, nickname, "MODE", srv);
	}
	if (!replyMsg.empty())
		srv->sendClient(fd, replyMsg);
}
