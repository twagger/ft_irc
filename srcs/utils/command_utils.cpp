#include <sstream>
#include <iostream>
#include <algorithm>
#include "../../includes/Server.hpp"
#include <sys/socket.h>

const std::vector<std::string> splitByComma(std::string parameter)
{
    std::vector<std::string> tab;
    std::string temp;
    std::istringstream stream(parameter);
    while (std::getline(stream, temp, ','))
    {
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        tab.push_back(temp);
    }
    return (tab);
}

// CHANNEL HELPERS

bool isChannel(std::string channelName)
{
    if (channelName.find("#") != std::string::npos
        || channelName.find("&") != std::string::npos)
        {
            return (true);
        }
    return (false);
}

std::string getChannelTopic(std::string channelName,
                        std::map<std::string, Channel *> channelList)
{
    std::map<std::string, Channel*>::iterator  i;

    i = channelList.find(channelName);
    if (i != channelList.end())
    {
        return (i->second->getTopic());
    }
    return (0);
}

std::vector<std::string> getChannelKey(std::vector<std::string> parameter)
{
    std::vector<std::string>::iterator it = parameter.end();

    return (splitByComma(*it));
}

std::map<std::string, Channel *>::iterator findChannel(std::map<std::string,
        Channel *> channelList, std::string channelName)
{
    std::map<std::string, Channel *>::iterator it = channelList.find(channelName);
    return (it);
}


// REPLY HELPERS

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
        nicknameList = nicknameList + (*it)->getNickname() + "  ";
    }
	std::string reply = ":" + irc->getHostname() + " " + code + " "
						+ irc->getUserByFd(fd)->getNickname() + "= "
                        + channelName + " :@" + nicknameList;		
	return (reply);
}

std::string eventChannel(Server *irc, const int &fd, std::string eventType,
    std::string channelName)
{
    std::string event = ":" + irc->getUserByFd(fd)->getNickname() + "!"
        + irc->getUserByFd(fd)->getUsername() + "@::" + irc->getHostname() + " "
        + eventType + channelName; 		// for @ shouldn't it be  irc->getUserByFd(fd)->getHostname() ?
    return (event);
}

std::string clientReply(Server *irc, const int &originFd, std::string replyMsg)
{
	std::string reply = ":" + irc->getUserByFd(originFd)->getNickname() + "!"
						+ irc->getUserByFd(originFd)->getUsername() + "@"
						+ irc->getUserByFd(originFd)->getHostname() + " " + replyMsg;
	return (reply);
}
