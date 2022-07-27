#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

int checkPartParameter(std::map<std::string, Channel *> channelList,
    std::string channelName, User *currentUser)
{
    // Not enough parameter
    if (channelName.empty() == true)
        return (-1);
    // Channel must exist
    std::map<std::string, Channel *>::iterator it = channelList.find(channelName);
    if (it == channelList.end())
        return (-2);
    // Current user must be part of the channel
    if (findUserOnChannel(it->second->_users, currentUser) == it->second->_users.end())
        return (-3);
    return (0);
}

const std::string part(const int &fdUser, const std::vector<std::string> &parameter, const std::string &, Server *server)
{
    std::vector<std::string> channel;
    std::string partMessage;

    channel = splitByComma(parameter[0]);
    if (parameter.size() > 1)
        partMessage = parameter[1];

    std::vector<std::string>::iterator it = channel.begin();
    // Check part parameters
    for (; it != channel.end(); it++)
    {
        if (checkPartParameter(server->_channelList, *it, server->getUserByFd(fdUser)) == -1)
            return (numericReply(server, fdUser, "461", ERR_NEEDMOREPARAMS(std::string("PART"))));
        else if (checkPartParameter(server->_channelList, *it, server->getUserByFd(fdUser)) == -2)
            return (numericReply(server, fdUser, "403", ERR_NOSUCHCHANNEL(*it)));
        else if (checkPartParameter(server->_channelList, *it, server->getUserByFd(fdUser)) == -3)
            return (numericReply(server, fdUser, "442", (*it)));

        // Effectively part from channel
        std::map<std::string, Channel *>::iterator channelPos = server->_channelList.find(*it);
        channelPos->second->removeUser(server->getUserByFd(fdUser));
        server->getUserByFd(fdUser)->removeChannelJoined(*it);
    }
    std::string channelName = *it;
    // Reply once user parted from channel
    return (clientReply(server, fdUser, "has left " + channelName + " " + partMessage));
}