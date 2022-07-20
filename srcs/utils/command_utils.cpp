#include "../../includes/utils.hpp"
#include <sstream>
#include <iostream>
#include "../server/Server.hpp"

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

std::string reply(std::string servername, std::string code, std::string nickname, std::string replyMsg)
{
	std::cout << ":" + servername + " " + code + " " +  nickname + " :" + replyMsg;		
	// string int should be a 3-digits number 
	// to be returned to the client send(new_fd, "\n\", 14, 0) == -1)
}