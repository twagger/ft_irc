#include "usercmds.hpp"
#include "../server/Server.hpp"
#include "utils.hpp"


// Registration functions for each client

// order:
// 1. Pass message
// 2. Nick message
// 3. User message

// replies:
// RPL_WELCOME

// PASS MUST precede NICK/USER combination for user connection


std::string	pass(Server *irc, const int &fd, std::string password) {
	if (irc->getUserByFd(fd) != 0)
	{
		if (password.empty())
			return reply(irc, fd, "461", ERR_NEEDMOREPARAMS(std::string("PASS")));
		else if (irc->getUserByFd(fd)->getPassword() == true)
			return reply(irc, fd, "462", ERR_ALREADYREGISTRED);
		else if (!irc->getUserByFd(fd)->getPassword() && irc->getPassword() == password) {	
			// never authenticated with a password + password given is OK
			irc->getUserByFd(fd)->setPassword(true);
			return (NULL);
		}
	}
	return (NULL);
}









// OPER: SQUIT, CONNECT, KILL

