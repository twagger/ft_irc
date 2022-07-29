#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

std::string	userModesToStr(User *user)
{
	std::string modes;

	modes.append("+");
	if (user->hasMode(MOD_RESTRICTED))
		modes.append("r");
	if (user->hasMode(MOD_SRVNOTICES))
		modes.append("s");
	if (user->hasMode(MOD_LOCALOP))
		modes.append("O");
	if (user->hasMode(MOD_OPER))
		modes.append("o");
	if (user->hasMode(MOD_INVISIBLE))
		modes.append("i");
	if (user->hasMode(MOD_WALLOPS))
		modes.append("w");
	if (user->hasMode(MOD_AWAY))
		modes.append("a");
	return (modes);
}

void addModes(User *user, const std::string mode, int start, int stop) {
	std::cout << "[DEBUG] ADD function step 1 - start " << start << " stop " << stop << std::endl;
	for (int i = start; i < stop; i++) {
			std::cout << "[DEBUG] ADD function step 2 - mode " << i << " is " << mode[i] << std::endl;
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

void removeModes(User *user, const std::string mode, int start, int stop) {
		std::cout << "[DEBUG] DELETE function step 1 - start " << start << " stop " << stop << std::endl;
	for (int i = start; i < stop; i++) {
		std::cout << "[DEBUG] DELETE function step 2 - mode " << i << " is " << mode[i] << std::endl;
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

void findPair(const std::string modes, unsigned int i, std::pair <char, int> *pair) {
	pair->first = modes[i];
	pair->second = i;

	for (i = i + 1 ; i < modes.size(); i++) {
		std::cout << "[DEBUG] find pair - i " << i  << " modes[i] " << modes[i] << std::endl;
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

	std::cout << "[DEBUG] ADD/neg step 1 " << std::endl;
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

void	mode(const int &fd, const std::vector<std::string> &params, const std::string &prefix, 
		Server *srv) {
	
	std::string replyMsg;
	User *user = srv->getUserByFd(fd);

	std::cout << "[DEBUG] Mode step 1 " << std::endl;
	if (params.empty() || params[0].empty()) {
			std::cout << "[DEBUG] Mode step 2 " << std::endl;
			replyMsg = numericReply(srv, fd, "461",
				ERR_NEEDMOREPARAMS(prefix));
	}
	else if (params.size() == 1 && params[0] == user->getNickname()) {		// no nickname given
			std::cout << "[DEBUG] Mode step 3 " << std::endl;	
		replyMsg = numericReply(srv, fd, "221", RPL_UMODEIS(userModesToStr(user)));
	}
	else if (srv->getUserByNickname(params[0]) == 0) {
			std::cout << "[DEBUG] Mode step 4 " << std::endl;
	   	replyMsg = numericReply(srv, fd, "401", ERR_NOSUCHNICK(params[0]));
	}
	else if (params[0] != user->getNickname()) {
			std::cout << "[DEBUG] Mode step 5 " << std::endl;
		replyMsg = numericReply(srv, fd, "502", ERR_USERSDONTMATCH);
	}
	// else if (params.find(USERMODES))									// replace with pattern match
	// 	replyMsg = numericReply(srv, fd, "501", ERR_UMODEUNKNOWNFLAG);		
	else if (!params[1].empty()) {
			std::cout << "[DEBUG] Mode step 6 " << std::endl;
		handleAddRemoveModes(user, params[1]);
	} 	// we already checked above that param == user's own nickname
	srv->sendClient(fd, replyMsg);
	return ;
}
