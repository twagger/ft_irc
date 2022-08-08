#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

/**
 * @brief Kick a user from a channel with or without a specific message.
 * 
 * Errors handled:
 * - ERR_NEEDMOREPARAMS
 * - ERR_NOTONCHANNEL
 * - ERR_USERNOTINCHANNEL
 * - ERR_NOSUCHCHANNEL
 * - ERR_CHANOPRIVSNEEDED
 *   
 */

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
                          server->getUserByFd(fdUser)) == false)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "442", ERR_NOTONCHANNEL(channel)));
        return (-2);
    }
    return (0);
}

int checkGeneralParameter(std::vector<std::string> channel, Server *server,
    const int &fdUser)
{
    // channelList must not be empty
    if (server->_channelList.empty() == true)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "403", ERR_NOSUCHCHANNEL(channel[0])));
        return (-1);
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
                              server->getUserByNickname(*itVector)) == false)
        {
            server->sendClient(fdUser, numericReply(server, fdUser,
                                                    "441", ERR_USERNOTINCHANNEL(*itVector, channel)));
            return;
        }
        // check if users on user list are operators.
        // In that case current user must be an operator
        if (findUserOnChannel(itMap->second->_operators,
                              server->getUserByNickname(*itVector)) == true &&
            findUserOnChannel(itMap->second->_operators,
                              server->getUserByFd(fdUser)) == false)

        {
            server->sendClient(fdUser, numericReply(server, fdUser,
                                                    "482", ERR_CHANOPRIVSNEEDED(channel)));
            return;
        }
        // effectively kick user
        server->sendChannel(channel, clientReply(server, fdUser,
            "KICK " + channel + " " + *itVector + " :" + kickMessage));
        itMap->second->removeUser(server->getUserByNickname(*itVector));
        server->getUserByNickname(*itVector)->removeChannelJoined(channel);
        // Check if the user was operator and remove him from the operator list
        if (findUserOnChannel(itMap->second->_operators,
            server->getUserByFd(fdUser)) == true)
            itMap->second->removeOperator(server->getUserByFd(fdUser));
            // Check if the user was invited and remove him from the invitee list
        if (findUserOnChannel(itMap->second->_invitees,
            server->getUserByFd(fdUser)) == true)
            itMap->second->removeInvitee(server->getUserByFd(fdUser));


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
    
    if (parameter.size() < 2)
        return (server->sendClient(fdUser, numericReply(server, fdUser,
            "461", ERR_NEEDMOREPARAMS(std::string("KICK")))));
    channel = splitByComma(parameter[0]);
    if (parameter.size() > 1)
        user = splitByComma(parameter[1]);
    if (parameter.size() > 2)
        kickMessage = parameter[2];
    if (checkGeneralParameter(channel, server, fdUser) < 0)
        return;
    if (channel.size() == 1)
        return (oneChannelCase(channel[0], user, kickMessage, server, fdUser));
    else
        return (multipleChannelCase(channel, user, kickMessage, server, fdUser));
}
