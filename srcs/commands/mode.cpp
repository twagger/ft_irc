#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

void handleMode(User *user, const std::string mode)
{
	if (mode[0] == '+') {
		switch (mode[1])
		{
			case 'a':
				break;
			case 'i':
				user->addMode(MOD_INVISIBLE);
				break;
			case 'w':
				user->addMode(MOD_WALLOPS);
				break;
			case 'r':
				user->addMode(MOD_RESTRICTED);
				break;
			case 'o':
				break;
			case 'O':
				break;
			case 's':
				user->addMode(MOD_SRVNOTICES);
				break;
		}
	}
	else if (mode[0] == '-') {
		switch (mode[1])
		{
			case 'a':
				break;
			case 'i':
				user->removeMode(MOD_INVISIBLE);
				break;
			case 'w':
				user->removeMode(MOD_WALLOPS);
				break;
			case 'r':
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
		}
	}
}


void mode(const int &fd, const std::vector<std::string> &params, const std::string &, 
		Server *srv) {
	
	User *user = srv->getUserByFd(fd);

	if (params.size() == 1)		// no nickname given
		handleMode(user, params[0]);
	else if (params[0] == user->getNickname()) 	// param == own nickname
		handleMode(user, params[1]);

}