#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"

bool	emptyParams(const std::vector<std::string> &params) {
	if (params.empty())
		return true;
	for (unsigned int i = 0; i < params.size(); i++) {
		if (params[i].empty()) 	
			return true;
	}
	return false;
}

std::string	numericReply(Server *irc, const int &fd, std::string code, std::string replyMsg)
{
	std::string reply = ":" + irc->getHostname() + " " + code + " "
						+ irc->getUserByFd(fd)->getNickname() + " " + replyMsg;		
	return (reply);
}

void	serverQuitNotice(const int &fd,  Server *srv, const std::string &destNick, std::string msg) 
{
	
	std::string reply = ":" + srv->getHostname() + " NOTICE " + destNick + " " + ":" + msg;
	srv->sendClient(fd, reply);
}

void 	informUsers(Server *srv, std::string msg) 
{
	std::deque<User*> users = srv->getAllUsers();
	std::deque<User*>::iterator it = users.begin();
	std::deque<User*>::iterator ite = users.end();

	for (; it != ite; it++)
		serverQuitNotice((*it)->getFd(), srv, (*it)->getNickname(), msg);
}

std::string WelcomeChan(Server *irc, const int &fd, std::string code,
    std::map<std::string, Channel *>::iterator itMap, std::string channelName)
{
    std::string nicknameList;
    std::deque<User *> listUser = itMap->second->_users;
    std::deque<User *> listOperator = itMap->second->_operators;
    std::deque<User *>::iterator itUser;
    std::deque<User *>::iterator itOperator;

    for (itUser = listUser.begin(); itUser != listUser.end(); itUser++)
    {
        // Case if user is an operator
        for (itOperator = listOperator.begin(); itOperator != listOperator.end();
            itOperator++)
        {
            if (*itUser == *itOperator)
                nicknameList += "@"; 
        }
        nicknameList += (*itUser)->getNickname() + " ";
    }
	std::string reply = ":" + irc->getHostname() + " " + code + " "
						+ irc->getUserByFd(fd)->getNickname() + " = "
                        + channelName + " ";
    reply += nicknameList + "\r\n";		
	return (reply);
}

std::string clientReply(Server *irc, const int &originFd, std::string replyMsg)
{
	std::string reply = ":" + irc->getUserByFd(originFd)->getNickname() + "!"
						+ irc->getUserByFd(originFd)->getUsername() + "@:"
						+ irc->getUserByFd(originFd)->getHostname() + " " + replyMsg
                        + "\r\n";
	return (reply);
}

std::vector<std::string> splitByComma(std::string parameter)
{
    std::vector<std::string> tab;

    // Case where no comma were found

    if (parameter.find(',') == std::string::npos)
    {
        tab.push_back(parameter);
        return (tab);
    }

    // Case where one comma was found

    std::string temp;
    std::istringstream stream(parameter);
    while (std::getline(stream, temp, ','))
    {
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        tab.push_back(temp);
    }
    return (tab);
}

bool isChannel(std::string channelName)
{
    if (channelName.find("#") != std::string::npos
        || channelName.find("&") != std::string::npos)
        {
            return (true);
        }
    return (false);
}

bool findUserOnChannel(std::deque<User *> userList, User *currentUser)
{
    std::deque<User *>::iterator it = userList.begin();

    for (; it != userList.end(); it++)
    {
        if (*it == currentUser)
            return (true);
    }
    return (false);
}
