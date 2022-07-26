#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

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

int checkPartParameter(std::map<std::string, Channel *> channelList,
    std::string channelName, User *currentUser)
{
    if (channelName.empty() == true)
        return (-1);
    std::map<std::string, Channel *>::iterator it = findChannel(channelList, channelName);
    if (it == channelList.end())
        return (-2);
    if (findUserOnChannel(it->second->_users, currentUser) != it->second->_users.end())
        return (-3);
}

const std::string part(const int &fdUser, const std::vector<std::string> &parameter, const std::string &, Server *server)
{
    std::vector<std::string> channel;

    channel = getChannelKey(parameter);

    std::vector<std::string>::iterator it = channel.begin();
    // Check part parameters
    for (; it != channel.end(); it++)
    {
        if (checkPartParameter(server->_channelList, *it, server->getUserByFd()) == -1)
            return (numericReply(server, fdUser, "461", ERR_NEEDMOREPARAMS(std::string("PART"))));
        else if (checkPartParameter(server->_channelList, *it, server->getUserByFd()) == -2)
            return (numericReply(server, fdUser, "403", ERR_NOSUCHCHANNEL(*it)));
        else if (checkPartParameter(server->_channelList, *it, server->getUserByFd()) == -3)
            return (numericReply(server, fdUser, "442", (*it)));

        // Effectively part from channel
        server->_channelList->_users->erase(findUserOnChannel(server->_channelList->_users,
            server->getUserByFd(fdUser)));
        server->getUserByFd(fdUser)->removeChannelJoined(*it);
    }
    std::string channelName = *it;
    return (eventChannel(server, fdUser, "PART", channelName));
    // Reply once user parted from channel
    std::string event = clientReply(server, fdUser, "has left " + channelName);
}