#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

int checkParameter(std::vector<std::string> channel, std::vector<std::string> user,
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
        return (-2);
    }
    // channel must exist
    std::vector<std::string>::iterator itVector = channel.begin();
    std::map<std::string, Channel *>::iterator itMap; 
    for (; itVector != channel.end(); itVector++)
    {
        itMap = server->_channelList.find(*itVector);
        if (itMap == server->_channelList.end())
            return (-3);
    }
    // user must exist

}

void oneChannelCase(std::vector<std::string> channel, std::vector<std::string> user,
    std::string kickMessage, Server *server, const int &fdUser)
{
    std::map<std::string, Channel *>::iterator itMap = server->_channelList.find(channel[0]);
    if (itMap == server->_channelList.end())
        return (numericReply(server, fdUser, "403", ERR_NOSUCHCHANNEL(channel[0])));
    std::vector<std::string>::iterator itVector = user.begin();

    for (; itVector != user.end(); itVector++)
    {
        
    }

}

void multipleChannelCase(std::vector<std::string> channel, std::vector<std::string> user,
    std::string kickMessage, Server *server, const int &fdUser)
{
    if (channel.size() != user.size())
        return ("ERROR");
}

void kick(const int &fdUser, const std::vector<std::string> &parameter, Server *server)
{
    std::vector<std::string> channel;
    std::vector<std::string> user;
    std::string kickMessage;

    channel = splitByComma(parameter[0]);
    if (parameter.size() > 1)
        user = splitByComma(parameter[1]);
    if (parameter.size() > 2)
        kickMessage = parameter[3];
    if (checkParameter(channel, user, server, fdUser) < 0)
        return ;
    if (channel.size() == 1)
        return (oneChannelCase(channel, user, kickMessage, server, fdUser));
    else
        return (multipleChannelCase(channel, user, kickMessage, server, fdUser));
}