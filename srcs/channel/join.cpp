#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

void createChannel(std::string channelName, int pos, std::vector<std::string> key,
                   User *currentUser, Server *server)
{
    if (key.empty() == false)
    {
        std::vector<std::string>::iterator it = key.begin() + pos;
        Channel *newChannel = new Channel(channelName, *it, currentUser);
        server->_channelList.insert(std::pair<std::string, Channel *>(channelName,
                                                                      newChannel));
        currentUser->addChannelJoined(channelName);
    }
    else
    {
        Channel *newChannel = new Channel(channelName, currentUser);
        server->_channelList.insert(std::pair<std::string, Channel *>(channelName,
                                                                      newChannel));
        currentUser->addChannelJoined(channelName);
    }
}

int checkKey(size_t pos, std::vector<std::string> key,
             std::map<std::string, Channel *>::iterator itMap, Server *server,
             const int &fdUser)
{
    if (key.empty() == true)
        return (0);
    std::string keySetInChannel = itMap->second->getKey();
    std::vector<std::string>::iterator it = key.begin() + pos;
    std::string keyToCheck = *it;

    if (keyToCheck.compare(keySetInChannel) != 0)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "475", ERR_BADCHANNELKEY(itMap->second->getChannelName())));
        return (-1);
    }
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

int checkChannel(std::string channel, Server *server, const int &fdUser)
{
    if (channel.empty() == true)
    {
        server->sendClient(fdUser, numericReply(server, fdUser, "461", ERR_NEEDMOREPARAMS(std::string("JOIN"))));
        return (-1);
    }
    if (channel.size() > 50)
    {
        server->sendClient(fdUser, numericReply(server, fdUser, "403", ERR_NOSUCHCHANNEL(channel)));
        return (-1);
    }
    if (isChannel(channel) == false || channel.find(',') != std::string::npos)
    {
        server->sendClient(fdUser, numericReply(server, fdUser, "476", ERR_BADCHANMASK(channel)));
        return (-1);
    }
    return (0);
}

void addUserToChannel(std::map<std::string, Channel *>::iterator itMap, User *userToAdd)
{
    itMap->second->addUser(userToAdd);
    userToAdd->addChannelJoined(itMap->second->getChannelName());
}

void join(const int &fdUser, const std::vector<std::string> &parameter, const std::string &, Server *server)
{
    // Get parameters of join
    std::vector<std::string> channel;
    std::vector<std::string> key;

    channel = splitByComma(parameter[0]);
    if (parameter.size() > 1)
        key = splitByComma(parameter[1]);

    std::vector<std::string>::iterator itChan = channel.begin();
    itChan = channel.begin();
    for (; itChan != channel.end(); itChan++)
    {
        // Check channelname
        if (checkChannel(*itChan, server, fdUser) < 0)
            return;
        // Case where channel already exists
        if (server->_channelList.empty() == false)
        {
            std::map<std::string, Channel *>::iterator itMap = server->_channelList.find(*itChan);
            if (itMap != server->_channelList.end())
            {
                if (checkKey(itChan - channel.begin(), key, itMap, server, fdUser) < 0)
                    return;
                std::vector<char>::iterator itMode = findMode(itMap->second->_mode, 'i');
                if (itMap->second->_mode.empty() == false && itMode != itMap->second->_mode.end() && checkInviteBan(itMap->second->_invitees, server->getUserByFd(fdUser)) < 0)
                    return (server->sendClient(fdUser, numericReply(server, fdUser,
                                                            "473", ERR_INVITEONLYCHAN(*itChan))));
                if (itMap->second->_bannedUsers.empty() == false && checkInviteBan(itMap->second->_bannedUsers, server->getUserByFd(fdUser)) < 0)
                    return (server->sendClient(fdUser, numericReply(server, fdUser,
                        "474", ERR_BANNEDFROMCHAN(*itChan))));
                addUserToChannel(itMap, server->getUserByFd(fdUser));
            }
            else
                createChannel(*itChan, itChan - channel.begin(), key,
                              server->getUserByFd(fdUser), server);
        }
        // Case where channel doesn't exist
        else
        {
            createChannel(*itChan, itChan - channel.begin(), key,
                          server->getUserByFd(fdUser), server);
        }
        // Reply if the user successfully joined the channel
        // Use send for all the user of a channel (vector of fd)
        std::string event = clientReply(server, fdUser, "has joined " + *itChan);
        std::string userList = replyList(server, fdUser, "353",
                                         server->_channelList.find(*itChan)->second->_users,
                                         *itChan);
        std::string endOfNames = numericReply(server, fdUser, "366", RPL_ENDOFNAMES(*itChan));
        server->sendChannel(*itChan, event);
        server->sendClient(fdUser, userList + "\r\n" + endOfNames);
    }
}
