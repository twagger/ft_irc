#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

/**
 * @brief View or change the topic of a channel
 * 
 * Errors handled:
 * - ERR_NEEDMOREPARAMS
 * - ERR_NOTONCHANNEL
 * - ERR_NOSUCHCHANNEL
 *   
 */

int checkTopicParameter(std::map<std::string, Channel *>::iterator itChannel,
    Server *server, const int &fdUser)
{
    User *currentUser = server->getUserByFd(fdUser);

    if (findUserOnChannel(itChannel->second->_users, currentUser) == false)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
            "442", ERR_NOTONCHANNEL(itChannel->first)));
        return (-2);
    }
    return (0);
}

int checkGeneralParameter(const int &fdUser, const std::vector<std::string> &parameter,
    Server *server, std::map<std::string, Channel *>::iterator itChannel)
{
    if (server->_channelList.empty() == true)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
            "461", ERR_NEEDMOREPARAMS(std::string("TOPIC"))));
        return (-1);
    }
    if (itChannel == server->_channelList.end())
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
            "403", ERR_NOSUCHCHANNEL(parameter[0])));
        return (-2);
    }
    return (0);
}


void changeTopic(std::string topic, std::map<std::string, Channel *>::iterator itChannel,
    Server *server, const int &fdUser)
{
    if (checkTopicParameter(itChannel, server, fdUser) < 0)
        return ;
    itChannel->second->setTopic(topic);
    return (server->sendChannel(itChannel->first, clientReply(server, fdUser,
        "TOPIC " + itChannel->first + " " + topic)));
}

void topic(const int &fdUser, const std::vector<std::string> &parameter,
    const std::string &, Server *server)
{
    std::string channel;
    std::map<std::string, Channel *>::iterator itChannel;
    
    if (parameter.size() < 1)
        return (server->sendClient(fdUser, numericReply(server, fdUser,
            "461", ERR_NEEDMOREPARAMS(std::string("TOPIC")))));
    channel = parameter[0];
    itChannel = server->_channelList.find(channel);
    if (checkGeneralParameter(fdUser, parameter, server, itChannel) < 0)
        return ;
    if (parameter.size() > 1)
        changeTopic(parameter[1], itChannel, server, fdUser);
    else if (itChannel->second->getTopic().empty() == false)
        server->sendClient(fdUser, numericReply(server, fdUser,
            "332", RPL_TOPIC(channel, itChannel->second->getTopic())));
    else
        server->sendClient(fdUser, numericReply(server, fdUser,
            "331", RPL_NOTOPIC(channel)));
}
