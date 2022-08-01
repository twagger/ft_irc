#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

int checkParameter(std::string topic, std::map<std::string, Channel *>::iterator itChannel,
    Server *server, const int &fdUser)
{
    // Topic format is not valid
    if (topic.find(':') != 0)
        return (-1);
    // Current user is not in user list
    User *currentUser = server->getUserByFd(fdUser);
    if (findUserOnChannel(itChannel->second->_users, currentUser) == itChannel->second->_users.end())
    {
        server->sendClient(fdUser, numericReply(server, fdUser, "442", ERR_NOTONCHANNEL(itChannel->first)));
        return (-2);
    }
}

void changeTopic(std::string topic, std::map<std::string, Channel *>::iterator itChannel,
    Server *server, const int &fdUser)
{
    if (checkParameter(topic, itChannel, server, fdUser) < 0)
        return ;
    itChannel->second->setTopic(topic);
    return (server->sendChannel(itChannel->first, clientReply(server, fdUser, "TOPIC " + itChannel->first + " " + topic)));
}

void topic(const int &fdUser, const std::vector<std::string> &parameter,
    const std::string &, Server *server)
{
    std::string channel;
    std::map<std::string, Channel *>::iterator itChannel;
    
    if (parameter.empty() == true)
        return (server->sendClient(fdUser, numericReply(server, fdUser, "461", ERR_NEEDMOREPARAMS(std::string("TOPIC")))));
    if (server->_channelList.empty() == true)
        return (server->sendClient(fdUser, numericReply(server, fdUser, "461", ERR_NEEDMOREPARAMS(std::string("TOPIC")))));
    channel = parameter[0];
    itChannel = server->_channelList.find(channel);
    if (parameter.size() > 1)
        changeTopic(parameter[1], itChannel, server, fdUser);
    else if (itChannel->second->getTopic().empty() == false)
        server->sendClient(fdUser, numericReply(server, fdUser, "332", RPL_TOPIC(channel, itChannel->second->getTopic())));
    else
        server->sendClient(fdUser, numericReply(server, fdUser, "331", RPL_NOTOPIC(channel)));
}
