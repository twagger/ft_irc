#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

/**
 * @brief Leave an existing channel
 * If all users part from the channel the channel will be destroyed
 * 
 * Errors handled:
 * - ERR_NEEDMOREPARAMS
 * - ERR_NOTONCHANNEL
 * - ERR_NOSUCHCHANNEL
 *   
 */

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
    if (findUserOnChannel(it->second->_users, currentUser) == false)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "442", ERR_NOTONCHANNEL(channelName)));
        return (-2);
    }
    return (0);
}

void checkChannelMustBeDeleted(Server *server,
                               std::map<std::string, Channel *>::iterator channelPos)
{
    Channel *channel = channelPos->second;

    if (channel->_users.empty() == true)
    {
        server->_channelList.erase(channelPos);
        delete channel;
    }
}

void part(const int &fdUser, const std::vector<std::string> &parameter,
          const std::string &, Server *server)
{
    std::vector<std::string> channel;
    std::string partMessage;
    Channel *currentChannel;
    std::map<std::string, Channel *>::iterator itMap;
    std::vector<std::string>::iterator it;

    if (parameter.size() < 1)
        return (server->sendClient(fdUser, numericReply(server, fdUser,
                                                        "461", ERR_NEEDMOREPARAMS(std::string("PART")))));
    channel = splitByComma(parameter[0]);
    // Not enough parameter
    // If channel list is empty, you can't part from any channel
    if (server->_channelList.empty() == true)
        return (server->sendClient(fdUser, numericReply(server, fdUser, "403",
                                                        ERR_NOSUCHCHANNEL(channel[0]))));

    if (parameter.size() > 1)
        partMessage = parameter[1];

    it = channel.begin();
    // Check part parameters
    for (; it != channel.end(); it++)
    {
        if (checkPartParameter(server->_channelList, *it, server->getUserByFd(fdUser),
                               server, fdUser) < 0)
            return;
        
        itMap = server->_channelList.find(*it);
        currentChannel = itMap->second;
        // Effectively part from channel
        server->sendChannel(*it, clientReply(server, fdUser, "PART " + *it + " :" + partMessage));
        currentChannel->removeUser(server->getUserByFd(fdUser));
        server->getUserByFd(fdUser)->removeChannelJoined(*it);
        // Check if the user was invited and remove him from the invitee list
        if (findUserOnChannel(currentChannel->_invitees,
            server->getUserByFd(fdUser)) == true)
            currentChannel->removeInvitee(server->getUserByFd(fdUser));
        // Check if the user was operator and remove him from the operator list
        if (findUserOnChannel(currentChannel->_operators,
            server->getUserByFd(fdUser)) == true)
            currentChannel->removeOperator(server->getUserByFd(fdUser));
        // If channel is empty it must be deleted
        checkChannelMustBeDeleted(server, itMap);
    }
}
