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


//What should we do if user send PASS pwd1 pwd2 ? Is it wrong?

std::string	pass(const int fd, std::vector<std::string> params, Server *irc) {
	
	std::string replyMsg;
	User *user = irc->getUserByFd(fd);

	if (user != 0)
	{
		if (params.empty() || params[0].empty()) {
			replyMsg = numericReply(irc, fd, "461",
				ERR_NEEDMOREPARAMS(std::string("PASS")));
		}
		else if (user->getPassword() == true) {
			replyMsg = numericReply(irc, fd, "462", ERR_ALREADYREGISTRED); 
		}
		else if (!user->getPassword() && irc->getPassword()
			== params[0]) {	
			// never authenticated with a password + password given is OK
			user->setPassword(true);
		}
	}
	// if wrong password but not authenticated, do nothing >> TO BE CHECKED
	// if user not find, will return null as well - should never happen as 
	// we create user for every fd
	return replyMsg;
}

// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
//  1 =>  A-Z / a-z ";" "[", "]", "\", "`", "_", "^", "{", "|", "}"
//  8 => A-Z / a-z 0/9 ";" "[", "]", "\", "`", "_", "^", "{", "|", "}", "-"

bool	forbiddenNick(std::string param)
{
    if (param.empty())
		return true;
	else if (!isalpha(param[0]) && std::string(NAMESPECIALS).find(param[0])
				== std::string::npos)										
				//&& param[0] != 92 ?
		return true;
	else if (param.length() > 9)
		return true;
	else {
		for (unsigned int i = 0; i < param.length(); i++) {
			if (!isalnum(param[i]) && std::string(NAMESPECIALS).find(param[i])
				== std::string::npos && param[i] != 45)
				return true;
		}
	}
	return false;
}

//NICk MUST BE DONE AFTER PWD
std::string nick(const int fd, std::vector<std::string> params, Server *irc) 
{	
	std::string replyMsg;
	User *user = irc->getUserByFd(fd);
	if (user != 0 && user->getPassword() == true)
	{
		if (params.empty() || params[0].empty()) {
			replyMsg = numericReply(irc, fd, "431", ERR_NONICKNAMEGIVEN);
		}
		else if (forbiddenNick(params[0]) == true) {
			replyMsg = numericReply(irc, fd, "432", ERR_ERRONEUSNICKNAME(params[0]));
		}
		// else if (irc->getUserByNick(params[0]) != 0) {								// waiting for getuserbynick()
		// 	replyMsg = numericReply(irc, fd, "433", ERR_NICKNAMEINUSE(params[0]));
		// }
		// else if (// nick is in kill list)											// waiting for killlist
		// 	replyMsg = numericReply(irc, fd, "437", ERR_UNAVAILRESOURCE(params[0]));
		else if (user->getMode() != 0 && user->getMode() == 3) {		// waiting for user mode in bitshift
			replyMsg = numericReply(irc, fd, "484", ERR_RESTRICTED);
		}
		else {
			user->setNickname(params[0]);
			replyMsg = clientReply(irc, fd, CLIENT_NICK(params[0]));
		}
		//authenticateUser(fd, irc, replyMsg);
	}
	return replyMsg;
}

// user =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF )
//     ; any octet except NUL, CR, LF, " " and "@"
//	0	00	NUL
//	10	0A	LF
//	13	0D	CR
//	32	20	' '
//	64	40	@
bool	forbiddenUsername(std::string param) 
{
    if (param.empty())
		return true;
	else if (param.find('\0') != std::string::npos)
		return true;
	else if (param.find(10) != std::string::npos)
		return true;
	else if (param.find(13) != std::string::npos)
		return true;
	else if (param.find(64) != std::string::npos)
		return true;
	return false;
}

bool areValidParams(std::vector<std::string> params) {
	
	// check params[0-3].empty() 
	if (forbiddenUsername(params[0]) || params[0].find(32) != std::string::npos)
		return false;
	else if (isdigit(params[1][0]) == 0 && (params[1][1] < 0 || params[1][1] > 7))
		return false;
	else if (params[2][0] != '*')
		return false;
	else if (params[3].size() < 1 && forbiddenUsername(params[3]))
		return false;
	return true;
}

std::string user(const int fd, std::vector<std::string> params, Server *irc)
{
	std::string replyMsg;
	User *user = irc->getUserByFd(fd);

	if (user != 0 && user->getPassword() == true) {
		if (params.empty() || params.size() < 4) {
			// should check for empty params here? => to be tested
			replyMsg = numericReply(irc, fd, "461",
				ERR_NEEDMOREPARAMS(std::string("USER")));
		}
		else if (!user->getUsername().empty()) {
			replyMsg = numericReply(irc, fd, "462", ERR_ALREADYREGISTRED); 
		}
		else if (areValidParams(params) == true) {
			user->setUsername(params[0]);
			user->setMode(params[1][0]);
			user->setFullname(params[3]);
		}
		//authenticateUser(fd, irc, replyMsg);
	}
	return (replyMsg);
}

bool	isAuthenticatable(User *user) 
{
	if (user->getAuthenticated() == true)
		return false;
	else if (user->getPassword() == false)
		return false;
	else if (user->getNickname().empty() || user->getUsername().empty())
		return false;
	return true;
}

std::string authenticateUser(const int fd, Server *irc, std::string replyMsg)
{
	User		*user = irc->getUserByFd(fd);

	if (isAuthenticatable(user)) {
		replyMsg.append(numericReply(irc, fd, "001",
			RPL_WELCOME(user->getNickname(), user->getUsername(), user->getHostname())));
		replyMsg.append(numericReply(irc, fd, "002",
			RPL_YOURHOST(irc->getHostname(), irc->getVersion())));
		replyMsg.append(numericReply(irc, fd, "003",
			RPL_CREATED(irc->getDate())));
		replyMsg.append(numericReply(irc, fd, "004",
			RPL_MYINFO(irc->getHostname(), irc->getVersion(), USERMODES, CHANNELMODES)));
		replyMsg.append(clientReply(irc, fd, 
			"Bienvenue - welcome - bouno vengudo - i bisimila - degemer mad - benvinguts - velkommen"));
		replyMsg.append(clientReply(irc, fd, "		           .       .                   .       .      .     .      ."));
		replyMsg.append(clientReply(irc, fd, "          .    .         .    .            .     ______                    "));
		replyMsg.append(clientReply(irc, fd, "      .           .             .               ////////"));
		replyMsg.append(clientReply(irc, fd, "                .    .   ________   .  .      /////////     .    ."));
		replyMsg.append(clientReply(irc, fd, "           .            |.____.  /\\        ./////////    ."));
		replyMsg.append(clientReply(irc, fd, "    .                 .//      \\/  |\\     /////////"));
		replyMsg.append(clientReply(irc, fd, "       .       .    .//          \\ |  \\ /////////       .     .   ."));
		replyMsg.append(clientReply(irc, fd, "                    ||.    .    .| |  ///////// .     ."));
		replyMsg.append(clientReply(irc, fd, "     .    .         ||           | |//`,/////                ."));
		replyMsg.append(clientReply(irc, fd, "             .      \\\\        ./ //  /  \\/   ."));
		replyMsg.append(clientReply(irc, fd, "  .                   \\\\.___./ //\\` '   ,_\\     .     ."));
		replyMsg.append(clientReply(irc, fd, "          .           .     \\ //////\\ , /   \\                 .    ."));
		replyMsg.append(clientReply(irc, fd, "                       .    ///////// \\|  '  |    ."));
		replyMsg.append(clientReply(irc, fd, "      .        .          ///////// .   \\ _ /          ."));
		replyMsg.append(clientReply(irc, fd, "                        /////////                              ."));
		replyMsg.append(clientReply(irc, fd, "                 .   ./////////     .     ."));
		replyMsg.append(clientReply(irc, fd, "         .           --------   .                  ..             ."));
		replyMsg.append(clientReply(irc, fd, "  .               .        .         .                       ."));
		replyMsg.append(clientReply(irc, fd, "                        ________________________"));
		replyMsg.append(clientReply(irc, fd, "____________------------                        -------------_________"));

	}
	return (replyMsg);
}
