#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

void createChannel(std::vector<std::string> channel, std::vector<std::string> key,
                   User *currentUser, Server *server)
{
    std::vector<std::string>::iterator it = channel.begin();
    std::vector<std::string>::iterator it2 = key.begin();

    for (; it != channel.end(); it++)
    {
        std::string channelName = *it;
        std::string channelKey = *it2;
        if (key.empty() == false || it2 != key.end())
        {
            Channel newChannel(channelName, channelKey, currentUser);
            server->_channelList.insert(std::pair<std::string, Channel*>(channelName,
                &newChannel));
            currentUser->addChannelJoined(channelName);
            it2++;
        }
        else
        {
            Channel newChannel(channelName, currentUser);
            server->_channelList.insert(std::pair<std::string, Channel*>(channelName,
                &newChannel));
            currentUser->addChannelJoined(channelName);
        }
    }
}

int checkKey(size_t pos, std::vector<std::string> key,
    std::map<std::string, Channel *>::iterator itMap)
{
    std::string keySetInChannel = itMap->second->getKey();
    std::vector<std::string>::iterator it = key.begin() + pos;
    std::string keyToCheck = *it;

    if (keyToCheck.compare(keySetInChannel) != 0)
        return (-1);
    return (0);
}

int checkInviteBan(std::deque<User *> listOfUser, User *currentUser)
{
    std::deque<User *>::iterator it = listOfUser.begin();

    for (; it != listOfUser.end(); it++)
    {
        if (currentUser == *it)
        {
            return (0);
        }
    }
    return (-1);
}

int checkChannel(std::string channel)
{
    if (channel.empty() == true)
        return (-1);
    if (channel.size() > 50)
        return (-2);
    if (isChannel(channel) == false || channel.find(',') != std::string::npos)
        return(-3);
    return (0);
}

const std::string join(const int &fdUser, const std::vector<std::string> &parameter, const std::string &, Server *server)
{
    // Get parameters of join
    std::vector<std::string> channel;
    std::vector<std::string> key;

    channel = getChannelKey(parameter);
    if (parameter.size() > 1)
        key = getChannelKey(parameter);

    std::vector<std::string>::iterator itChan = channel.begin();
    itChan = channel.begin();
    for (; itChan != channel.end(); itChan++)
    {
        //Check channelname
        std::string channelName = *itChan;
        if (checkChannel(channelName) == -1)
            return (numericReply(server, fdUser, "461", ERR_NEEDMOREPARAMS(std::string("JOIN"))));
        else if (checkChannel(channelName) == -2)
            return (numericReply(server, fdUser, "403", ERR_NOSUCHCHANNEL(channelName)));
        else if (checkChannel(channelName) == -3)
            return (numericReply(server, fdUser, "476", ERR_BADCHANMASK(channelName)));
        
        // Case where channel already exists
        std::map<std::string, Channel *>::iterator itMap = findChannel(server->_channelList,
            channelName);
        if (itMap != server->_channelList.end())
        {
            if (checkKey(itChan - channel.begin(), key, itMap) < 0)
                return (numericReply(server, fdUser, "475", ERR_BADCHANNELKEY(channelName)));
            if (checkInviteBan(itMap->second->_invitees, server->getUserByFd(fdUser)) < 0)
                return (numericReply(server, fdUser, "473", ERR_INVITEONLYCHAN(channelName)));
            if (checkInviteBan(itMap->second->_bannedUsers, server->getUserByFd(fdUser)) <  0)
                return (numericReply(server, fdUser, "474", ERR_BANNEDFROMCHAN(channelName)));
        }

        // Case where channel doesn't exist
        else
            createChannel(channel, key, server->getUserByFd(fdUser), server);
    }

    // Reply if the user successfully joined the channel
    std::string channelName = *itChan;
    std::string event = clientReply(server, fdUser, "has joined " + channelName);
    std::string userList = replyList(server, fdUser, "353",
        findChannel(server->_channelList, channelName)->second->_users,
        channelName);
    std::string endOfNames = numericReply(server, fdUser, "366", RPL_ENDOFNAMES(channelName));
    // Use send for all the user of a channel (vector of fd)
    return (event + "\r\n" + userList + "\r\n" + endOfNames);
}
