#include <sstream>
#include <iostream>
#include <algorithm>
#include "../../includes/Server.hpp"
#include <sys/socket.h>

bool isChannel(std::string channelName)
{
    if (channelName.find("#") != std::string::npos
        || channelName.find("&") != std::string::npos)
        {
            return (true);
        }
    return (false);
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

std::string numericReply(Server *irc, const int &fd, std::string code, std::string replyMsg)
{
	std::string reply = ":" + irc->getHostname() + " " + code + " "
						+ irc->getUserByFd(fd)->getNickname() + " " + replyMsg;		
	return (reply);
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
                std::cout << "coucou" << std::endl;
                // nicknameList += "@"; 
        }
        nicknameList += (*itUser)->getNickname() + " ";
        std::cout << "nicklist = " << nicknameList << std::endl;
    }
	std::string reply = ":" + irc->getHostname() + " " + code + " "
						+ irc->getUserByFd(fd)->getNickname() + " = "
                        + channelName;
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

std::deque<User *>::iterator findUserOnChannel(std::deque<User *> userList, User *currentUser)
{
    std::deque<User *>::iterator it = userList.begin();

    for (; it != userList.end(); it++)
    {
        if (*it == currentUser)
            return (it);
    }
    return (it);
}

std::vector<char>::iterator findMode(std::vector<char> listMode, char mode)
{
    std::vector<char>::iterator it = listMode.begin();
    for (; it != listMode.end(); it++)
    {
        if (*it == mode)
            return (it);
    }
    return (it);
}
