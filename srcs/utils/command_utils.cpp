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

std::string replyList(Server *irc, const int &fd, std::string code,
    std::deque<User *> userList, std::string channelName)
{
    std::string nicknameList;
    for (std::deque<User *>::iterator it = userList.begin(); it != userList.end(); it++)
    {
        nicknameList = nicknameList + (*it)->getNickname() + " ";
    }
	std::string reply = ":" + irc->getHostname() + " " + code + " "
						+ irc->getUserByFd(fd)->getNickname() + " = "
                        + channelName + " :@" + nicknameList + "\r\n";		
	return (reply);
}

std::string clientReply(Server *irc, const int &originFd, std::string replyMsg)
{
	std::string reply = ":" + irc->getUserByFd(originFd)->getNickname() + "!"
						+ irc->getUserByFd(originFd)->getUsername() + "@"
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
