#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

int checkChannelExist(std::string channel, const int &fdUser, Server *server)
{
    // channel must exist
    std::map<std::string, Channel *>::iterator itMap;
    itMap = server->_channelList.find(channel);
    if (itMap == server->_channelList.end())
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "403", ERR_NOSUCHCHANNEL(channel)));
        return (-1);
    }
    // current user must be in channel
    if (findUserOnChannel(itMap->second->getUsers(),
                          server->getUserByFd(fdUser)) == itMap->second->getUsers().end())
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "442", ERR_NOTONCHANNEL(channel)));
        return (-2);
    }
    return (0);
}

int checkGeneralParameter(std::vector<std::string> channel, std::vector<std::string> user,
                          Server *server, const int &fdUser)
{
    // user and channel must not be empty
    if (user.empty() == true || channel.empty() == true)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "461", ERR_NEEDMOREPARAMS(std::string("KICK"))));
        return (-1);
    }
    // channelList must not be empty
    if (server->_channelList.empty() == true)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "403", ERR_NOSUCHCHANNEL(channel[0])));
        return (-2);
    }
    return (0);
}

void oneChannelCase(std::string channel, std::vector<std::string> user,
                    std::string kickMessage, Server *server, const int &fdUser)
{
    if (checkChannelExist(channel, fdUser, server) < 0)
        return;
    std::vector<std::string>::iterator itVector = user.begin();
    std::map<std::string, Channel *>::iterator itMap = server->_channelList.find(channel);

    for (; itVector != user.end(); itVector++)
    {
        // users on user list must be in channel
        if (findUserOnChannel(itMap->second->getUsers(),
                              server->getUserByNickname(*itVector)) == itMap->second->getUsers().end())
        {
            server->sendClient(fdUser, numericReply(server, fdUser,
                                                    "441", ERR_USERNOTINCHANNEL(*itVector, channel)));
            return;
        }
        // check if users on user list are operators.
        // In that case current user must be an operator
        if (findUserOnChannel(itMap->second->_operators,
                              server->getUserByNickname(*itVector)) != itMap->second->_operators.end() &&
            findUserOnChannel(itMap->second->_operators,
                              server->getUserByFd(fdUser)) == itMap->second->_operators.end())

        {
            server->sendClient(fdUser, numericReply(server, fdUser,
                                                    "482", ERR_CHANOPRIVSNEEDED(channel)));
            return;
        }
        // effectively kick user
        server->sendChannel(channel, clientReply(server, fdUser, *itVector +
        "KICK " + channel + " " + kickMessage));
        itMap->second->removeUser(server->getUserByNickname(*itVector));
        server->getUserByNickname(*itVector)->removeChannelJoined(channel);
    }
}

void multipleChannelCase(std::vector<std::string> channel, std::vector<std::string> user,
                         std::string kickMessage, Server *server, const int &fdUser)
{
    if (channel.size() > user.size())
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "441", ERR_USERNOTINCHANNEL(user[0], channel[0])));
        return;
    }
    else
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "403", ERR_NOSUCHCHANNEL(channel[0])));
        return;
    }
    std::vector<std::string>::iterator itVector = channel.begin();
    for (; itVector != channel.end(); itVector++)
    {
        oneChannelCase(*itVector, user, kickMessage, server, fdUser);
    }
}

void kick(const int &fdUser, const std::vector<std::string> &parameter, const std::string &, Server *server)
{
    std::vector<std::string> channel;
    std::vector<std::string> user;
    std::string kickMessage;

    channel = splitByComma(parameter[0]);
    if (parameter.size() > 1)
        user = splitByComma(parameter[1]);
    if (parameter.size() > 2)
        kickMessage = parameter[3];
    if (checkGeneralParameter(channel, user, server, fdUser) < 0)
        return;
    if (channel.size() == 1)
        return (oneChannelCase(channel[0], user, kickMessage, server, fdUser));
    else
        return (multipleChannelCase(channel, user, kickMessage, server, fdUser));
}
