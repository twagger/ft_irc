#include "../../includes/usercmds.hpp"
#include "../../includes/utils.hpp"


// Registration functions for each client

// order:
// 1. Pass message
// 2. Nick message
// 3. User message

// replies:
// RPL_WELCOME

// PASS MUST precede NICK/USER combination for user connection

// rules for nickname / username:
// user       =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF )
//                  ; any octet except NUL, CR, LF, " " and "@"



//What should we do if user send PASS pwd1 pwd2 ? Is it wrong?

std::string	pass(int fd, std::vector<std::string> params, Server *irc) {
	std::string replyMsg;
	if (irc->getUserByFd(fd) != 0)
	{
		if (params.empty()) {
			replyMsg = reply(irc, fd, "461", ERR_NEEDMOREPARAMS(std::string("PASS")));
			return replyMsg;
		}
		else if (irc->getUserByFd(fd)->getPassword() == true) {
			replyMsg = reply(irc, fd, "462", ERR_ALREADYREGISTRED); 
			return replyMsg;
		}
		else if (!irc->getUserByFd(fd)->getPassword() && irc->getPassword() == params[0]) 
		{	
			// never authenticated with a password + password given is OK
			irc->getUserByFd(fd)->setPassword(true);
			return replyMsg;
		}
	}
	// if wrong password but not authenticated, do nothing >> TO BE CHECKED
	// if user not find, will return null as well
	return replyMsg;
}


std::string nick(int fd, std::vector<std::string> params, Server *irc) {
	if (irc->getUserByFd(fd) != 0)
	{
		if (params.empty())
	}
	return (NULL);
}






// OPER: SQUIT, CONNECT, KILL

