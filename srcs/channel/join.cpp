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
        // part();
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

std::string join(const int fdUser, std::vector<std::string> parameter, Server *server)
{
    // Check parameters of join

    std::vector<std::string> channel;
    std::vector<std::string> key;

    channel = getChannel(parameter);
    std::vector<std::string>::iterator it = channel.begin();
    std::string tmp = *it;
    if (tmp.compare("0") == 0)
    {
        return (std::string("PART"));
    }
    if (channel.empty() == true)
    {
        return (reply(server, fdUser, "461", ERR_NEEDMOREPARAMS(std::string("JOIN"))));
    }
    if (parameter.size() > 1)
    {
        key = getKey(parameter);
    }

    // Check if channel already exists
    it = channel.begin();
    for (;; it++)
    {
        std::string channelName = *it;
        std::string channelTopic = getChannelTopic(channelName, server->_channelList);
        std::map<std::string, Channel *>::iterator itMap = channelAlreadyExists(server->_channelList,
            channelName);
        if (itMap != server->_channelList.end() && channelTopic.empty() == false)
        {
            return (reply(server, fdUser, "332",
                    RPL_TOPIC(channelName, channelTopic)));
        }
        // Create channel
        else if (itMap == server->_channelList.end())
        {
            createChannel(channel, key, server->getUserByFd(fdUser), server);
        }
        return (reply(server, fdUser, "331", RPL_NOTOPIC(channelName)));
    }
} 