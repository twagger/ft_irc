#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

std::string	userModesToStr(User *user)
{
	std::string modes;

	modes.append("+");
	if (user->hasMode(MOD_AWAY))
		modes.append("a");
	if (user->hasMode(MOD_WALLOPS))
		modes.append("w");
	if (user->hasMode(MOD_INVISIBLE))
		modes.append("i");
	if (user->hasMode(MOD_OPER))
		modes.append("o");
	if (user->hasMode(MOD_SRVNOTICES))
		modes.append("s");
	if (user->hasMode(MOD_RESTRICTED))
		modes.append("r");
	return (modes);
}

void addModes(User *user, const std::string mode, int start, int stop) {
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
			case 's':
				user->addMode(MOD_SRVNOTICES);
				break;
			case 'r':
				user->addMode(MOD_RESTRICTED);
				break;
		}
	}
}

void removeModes(User *user, const std::string mode, int start, int stop) {
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
			case 's':
				user->removeMode(MOD_SRVNOTICES);
				break;
			case 'r':
				break;
		}
	}
}

void findPair(const std::string modes, unsigned int i, std::pair <char, int> *pair) {
	pair->first = modes[i];
	pair->second = i;

	for (i = i + 1 ; i < modes.size(); i++) {
		if (modes[i] == '+' || modes[i] == '-')
			break ;
		else
			pair->second++;
	}
	return ;
}

void	handleAddRemoveModes(User *user, const std::string modes)
{
	std::pair <char, int> pair;	

	for (unsigned int i = 0; i < modes.size(); i++) {
		if (modes[i] == '+' || modes[i] == '-') {
			findPair(modes, i, &pair);
		}		
		if (modes[i] == '+' && pair.second > 0) {
			addModes(user, modes, i, pair.second + 1);
		}
		else if (modes[i] == '-' && pair.second > 0) {
			removeModes(user, modes, i, pair.second + 1);
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
	else if (params.size() == 1 && params[0] == user->getNickname()) {		// no nickname given
		replyMsg = numericReply(srv, fd, "221", RPL_UMODEIS(userModesToStr(user)));
	}
	else if (srv->getUserByNickname(params[0]) == 0) {
	   	replyMsg = numericReply(srv, fd, "401", ERR_NOSUCHNICK(params[0]));
	}
	else if (params[0] != user->getNickname()) {
		replyMsg = numericReply(srv, fd, "502", ERR_USERSDONTMATCH);
	}
	// else if (isNotUserMode(params[1], USERMODES) || isNotChannelMode(params[1], CHANNELMODES))									// replace with pattern matching
	// 	replyMsg = numericReply(srv, fd, "501", ERR_UMODEUNKNOWNFLAG);		
	else if (!params[1].empty()) {
		handleAddRemoveModes(user, params[1]);
	} 	// we already checked above that param == user's own nickname
	srv->sendClient(fd, replyMsg);
	return ;
}
