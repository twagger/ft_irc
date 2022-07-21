#include <sstream>
#include <iostream>
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
    std::string temp;
    std::istringstream stream(parameter);
    while (std::getline(stream, temp, ','))
    {
        tab.push_back(temp);
    }
    return (tab);
}

/**
 * @brief Convert a vector of strings into a vector of Command by splitting 
 *        parameters and command
 * 
 * @param cmd_strings Vector of commands, not splitted by space
 * @return vector of Command where [ PREFIX ], CMD and PARAMS are separated
 */
std::vector<Command>  splitCmds(std::vector<std::string> cmd_strings)
{
    std::vector<Command>                result;
    std::vector<std::string>::iterator  it;
    int                                 end;
    std::string                         prefix;

    for (it = cmd_strings.begin(); it != cmd_strings.end(); ++it)
    {
        // extract CMD name 
        end = it->find(' ');
        if (end == -1) // only the command
        {
            if (it->find(':') == 0)
                throw std::runtime_error("IRC message must have a command");
            result.push_back(*(new Command(it->substr(0, end))));
        }
        else // command + params
        {
            // CMD WITH PREFIX
            if (it->find(':') == 0)
            {
                prefix = it->substr(1, end);
                it->erase(0, end + 1);
                end = it->find(' ');
                if (end == -1)
                    result.push_back(*(new Command(*it, prefix)));
                else
                    result.push_back(*(new Command(it->substr(0, end), prefix)));
                it->erase(0, end + 1);
            }
            else // CMD WITH NO PREFIX
            { 
                result.push_back(*(new Command(it->substr(0, end))));
                it->erase(0, end + 1);
            }
            // PARAMS
            while (end != -1)
            {
                end = it->find(' ');
                if (end == -1)
                    result.back().params.push_back(*it);
                else
                    result.back().params.push_back(it->substr(0, end));
                it->erase(0, end + 1);
            }
        }
    }
    
    return (result);
}

std::string reply(Server *irc, const int &fd, std::string code, std::string replyMsg)
{
	std::string reply = ":" + irc->getHostname() + " " + code + " "
						+ irc->getUserByFd(fd)->getNickname() + " :" + replyMsg;		
	return (reply);
}
