#include "../../includes/utils.hpp"
#include <sstream>
#include <iostream>
#include "../server/Server.hpp"
#include <sys/socket.h>

bool is_channel(std::string channelName)
{
    if (channelName.find("#") != std::string::npos
        || channelName.find("&") != std::string::npos)
        {
            return (true);
        }
    return (false);
}

std::vector<std::string> split_by_comma(std::string parameter)
{
    std::vector<std::string> tab;
    std::string temp;
    std::istringstream stream(parameter);
    while (std::getline(stream, temp, ','))
    {
        tab.push_back(temp);
    }
    return (tab);
}


std::string reply(Server *irc, const int &fd, std::string code, std::string replyMsg)
{
	std::string reply = ":" + irc->getHostname() + " " + code + " "
						+ irc->getUserByFd(fd)->getNickname() + " :" + replyMsg;		
	return (reply);
}