#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

std::string	userModesToStr(User *user)
{
	std::string modes;

	modes.append("+");
	if (user->getMode() - MOD_RESTRICTED > 0)
		modes.append("r");
	if (user->getMode() - MOD_SRVNOTICES > 0)
		modes.append("s");
	if (user->getMode() - MOD_LOCALOP > 0)
		modes.append("O");
	if (user->getMode() - MOD_OPER > 0)
		modes.append("o");
	if (user->getMode() - MOD_INVISIBLE > 0)
		modes.append("i");
	if (user->getMode() - MOD_WALLOPS > 0)
		modes.append("w");
	if (user->getMode() - MOD_AWAY > 0)
		modes.append("a");
	return (modes);
}

void addMode(User *user, const std::string mode, int start, int stop) {
	for (int i = start; i < stop; i++) {
		switch (mode[i])
		{
			case 'a':
				break;
			case 'w':
				user->addMode(MOD_WALLOPS);
				break;
			case 'i':
				user->addMode(MOD_INVISIBLE);
				break;
			case 'o':
				break;
			case 'O':
				break;
			case 's':
				user->addMode(MOD_SRVNOTICES);
				break;
			case 'r':
				user->addMode(MOD_RESTRICTED);
				break;
		}
	}
}

void removeMode(User *user, const std::string mode, int start, int stop) {
	for (int i = start; i < stop; i++) {
		switch (mode[i])
		{
			case 'a':
				break;
			case 'w':
				user->removeMode(MOD_WALLOPS);
				break;
			case 'i':
				user->removeMode(MOD_INVISIBLE);
				break;
			case 'o':
				user->removeMode(MOD_OPER);
				break;
			case 'O':
				user->removeMode(MOD_LOCALOP);
				break;
			case 's':
				user->removeMode(MOD_SRVNOTICES);
				break;
			case 'r':
				break;
		}
	}
}

void	handleAddRemoveModes(User *user, const std::string modes)
{
	int add;
	int rem;
	int end;

	for (unsigned int i = 0; i < modes.size(); i++) {
		if (modes[i] == '+') {
			add = ( modes.find('+') != std::string::npos ? modes.find('+') : modes.size() );
			rem = ( modes.find('-') != std::string::npos ? modes.find('-') : modes.size() );
			end = ( add < rem ? add : rem );
			addMode(user, modes, i + 1, end);
		}
		else if (modes[i] == '-') {
			add = ( modes.find('+') != std::string::npos ? modes.find('+') : modes.size() );
			rem = ( modes.find('-') != std::string::npos ? modes.find('-') : modes.size() );
			end = ( add < rem ? add : rem );
			removeMode(user, modes, i + 1, end);
		}
	}
}

void	mode(const int &fd, const std::vector<std::string> &params, const std::string &, 
		Server *srv) {
	
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	if (params.empty() || params[0].empty()) {
			replyMsg = numericReply(srv, fd, "461",
				ERR_NEEDMOREPARAMS(std::string("MODE")));
	}
	else if (params.size() == 1 && params[0] == user->getNickname())		// no nickname given
		replyMsg = numericReply(srv, fd, "221", RPL_UMODEIS(userModesToStr(user)));
	else if (srv->getUserByNickname(params[0]) == 0) 
	   	replyMsg = numericReply(srv, fd, "401", ERR_NOSUCHNICK(params[0]));
	else if (params[0] != user->getNickname())
		replyMsg = numericReply(srv, fd, "502", ERR_USERSDONTMATCH);
	// else if (params.find(USERMODES))									// replace with pattern match
	// 	replyMsg = numericReply(srv, fd, "501", ERR_UMODEUNKNOWNFLAG);		
	else if (!params[1].empty()) {
		handleAddRemoveModes(user, params[1]);
	} 	// we already checked above that param == user's own nickname
	return ;
}

