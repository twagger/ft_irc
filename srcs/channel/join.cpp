#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"

std::vector<std::string> getChannel(std::vector<std::string> parameter)
{
    std::vector<std::string>::iterator it = parameter.begin();
    std::vector<std::string> channel;

    // Case if parameter given is a 0
    std::string tmp = *it;
    if (tmp.compare("0") == 0)
    {
        channel.push_back("0");
        return (channel);
    }
    channel = splitByComma(tmp);
    for (it = channel.begin(); it != channel.end(); it++)
    {
        if (isChannel(*it) == false)
        {
            channel.erase(it);
        }
    }
    return (channel);
}

std::vector<std::string> getKey(std::vector<std::string> parameter)
{
    std::vector<std::string>::iterator it = parameter.end();

    return (splitByComma(*it));
}

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
            server->_channelList.insert(std::pair<std::string, Channel*>(channelName, &newChannel));
            it2++;
            // server->_userList.
        }
        else
        {
            Channel newChannel(channelName, currentUser);
            server->_channelList.insert(std::pair<std::string, Channel*>(channelName, &newChannel));
        }
    }
}

std::map<std::string, Channel *>::iterator channelAlreadyExists(std::map<std::string,
        Channel *> channelList, std::string channelName)
{
    std::map<std::string, Channel *>::iterator it = channelList.find(channelName);
    return (it);
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

int checkInvite(std::map<std::string, Channel *>::iterator itMap, User *currentUser)
{
    std::deque<User *> listOfInvitees = itMap->second->_invitees;
    std::deque<User *>::iterator it = listOfInvitees.begin();
}

int checkBan(std::map<std::string, Channel *>::iterator itMap, User *currentUser)
{

}


std::string join(const int fdUser, std::vector<std::string> parameter, Server *server)
{
    // Check parameters of join
    std::vector<std::string> channel;
    std::vector<std::string> key;

    channel = getChannel(parameter);
    std::vector<std::string>::iterator itChan = channel.begin();
    std::string tmp = *itChan;
    if (tmp.compare("0") == 0)
    {
        // function to part from all channels, peut etre ajouter une liste de channel auxquelle le user appartient
        return (std::string("PART"));
    }
    if (channel.empty() == true)
        return (reply(server, fdUser, "461", ERR_NEEDMOREPARAMS(std::string("JOIN"))));
    if (parameter.size() > 1)
        key = getKey(parameter);

    // Join channel
    itChan = channel.begin();
    for (; itChan != channel.end(); itChan++)
    {
        std::string channelName = *itChan;
        std::string channelTopic = getChannelTopic(channelName, server->_channelList);
        std::map<std::string, Channel *>::iterator itMap = channelAlreadyExists(server->_channelList,
            channelName);
        // Case where channel already exists
        if (itMap != server->_channelList.end())
        {
            if (checkKey(itChan - channel.begin(), key, itMap) < 0)
                return (reply(server, fdUser, "475", ERR_BADCHANNELKEY(channelName)));
            if (checkInvite(itMap, server->getUserByFd(fdUser)) < 0)
                return (reply(server, fdUser, "473", ERR_INVITEONLYCHAN(channelName)));
            if (checkBan(itMap, server->getUserByFd(fdUser)) <  0)
                return (reply(server, fdUser, "474", ERR_BANNEDFROMCHAN(channelName)));
        }
        // Case where channel doesn't exist
        else if (itMap == server->_channelList.end())
            createChannel(channel, key, server->getUserByFd(fdUser), server);
    }
    std::string channelName = *itChan;
    std::string channelTopic = getChannelTopic(channelName, server->_channelList);
    if (channelTopic.empty() == false)
        return (reply(server, fdUser, "332", RPL_TOPIC(channelName, channelTopic)));
    return (reply(server, fdUser, "331", RPL_NOTOPIC(channelName)));
}