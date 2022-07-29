#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

int checkPartParameter(std::map<std::string, Channel *> channelList,
                       std::string channelName, User *currentUser, Server *server, const int &fdUser)
{
    // Channel must exist
    std::map<std::string, Channel *>::iterator it = channelList.find(channelName);
    if (it == channelList.end())
    {
        server->sendClient(fdUser, numericReply(server, fdUser, "403",
                                                ERR_NOSUCHCHANNEL(channelName)));
        return (-1);
    }
    // Current user must be part of the channel
    if (findUserOnChannel(it->second->_users, currentUser) == it->second->_users.end())
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "442", ERR_NOTONCHANNEL(channelName)));
        return (-2);
    }
    return (0);
}

void part(const int &fdUser, const std::vector<std::string> &parameter,
          const std::string &, Server *server)
{
    std::vector<std::string> channel;
    std::string partMessage;

    channel = splitByComma(parameter[0]);
    // Not enough parameter
    if (channel.empty() == true)
        return (server->sendClient(fdUser, numericReply(server, fdUser,
                                                        "461", ERR_NEEDMOREPARAMS(std::string("PART")))));
    // If channel list is empty, you can't part from any channel
    if (server->_channelList.empty() == true)
        return (server->sendClient(fdUser, numericReply(server, fdUser, "403",
                                                        ERR_NOSUCHCHANNEL(channel[0]))));

    if (parameter.size() > 1)
        partMessage = parameter[1];

    std::vector<std::string>::iterator it = channel.begin();
    // Check part parameters
    for (; it != channel.end(); it++)
    {
        if (checkPartParameter(server->_channelList, *it, server->getUserByFd(fdUser),
                               server, fdUser) < 0)
            return;
        // Effectively part from channel
        std::map<std::string, Channel *>::iterator channelPos = server->_channelList.find(*it);
        channelPos->second->removeUser(server->getUserByFd(fdUser));
        server->getUserByFd(fdUser)->removeChannelJoined(*it);
        // Reply once user parted from channel
        server->sendChannel(*it, clientReply(server, fdUser, "PART " + *it + " :" + partMessage));
    }
}
