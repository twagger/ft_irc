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

std::string	pass(const int fd, std::vector<std::string> params, Server *irc) {
	
	std::string replyMsg;
	
	if (irc->getUserByFd(fd) != 0)
	{
		if (params.empty() || params.front().empty()) {
			replyMsg = reply(irc, fd, "461",
				ERR_NEEDMOREPARAMS(std::string("PASS")));
		}
		else if (irc->getUserByFd(fd)->getPassword() == true) {
			replyMsg = reply(irc, fd, "462", ERR_ALREADYREGISTRED); 
		}
		else if (!irc->getUserByFd(fd)->getPassword() && irc->getPassword()
			== params.front()) {	
			// never authenticated with a password + password given is OK
			irc->getUserByFd(fd)->setPassword(true);
		}
	}
	// if wrong password but not authenticated, do nothing >> TO BE CHECKED
	// if user not find, will return null as well
	return replyMsg;
}

bool	forbiddenCharInName(std::string param) {

//	char null = 0;		//0		00	NUL
//	char lf = 10;		//10	0A	LF
//	char cr = 13;		//13	0D	CR
//	char zero = 48;		//48	30	0
//	char at = 64;		//64	40	@
    if (param.find('\0') != std::string::npos)
		return true;
	else if (param.find(10) != std::string::npos)
		return true;
	else if (param.find('\f') != std::string::npos)
		return true;
	else if (param.find(13) != std::string::npos)
		return true;
	else if (param.find(48) != std::string::npos)
		return true;
	else if (param.find(64) != std::string::npos)
		return true;
	return false;
}


//NICk MUST BE DONE AFTER PWD
std::string nick(const int fd, std::vector<std::string> params, Server *irc) {
	
	std::cout << "test " << params[0] << std::endl;
	std::string replyMsg;
	if (irc->getUserByFd(fd) != 0 && irc->getUserByFd(fd)->getPassword() == true)
	{
		if (params.empty() || params.front().empty()) {
			replyMsg = reply(irc, fd, "431", ERR_NONICKNAMEGIVEN);
		}
		else if (forbiddenCharInName(params.front()) == true) {
			replyMsg = reply(irc, fd, "432", ERR_ERRONEUSNICKNAME(params.front()));
		}
		// else if (irc->getUserByNick(params.front())) {								// waiting for getuserbynick()
		// 	replyMsg = reply(irc, fd, "433", ERR_NICKNAMEINUSE(params.front()));
		// }
		//else if ()

	}
	return replyMsg;
}






// OPER: SQUIT, CONNECT, KILL

