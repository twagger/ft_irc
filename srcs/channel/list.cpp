#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

/**
 * @brief List channels with their topics
 * 
 * No error handled
 *   
 */

void listAllChannel(const int &fdUser, Server *server)
{
    std::map<std::string, Channel *>::iterator itChannel;

    for (itChannel = server->_channelList.begin();
        itChannel != server->_channelList.end(); itChannel++)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
            "322", RPL_LIST(itChannel->first, itChannel->second->getTopic())));
    }
    server->sendClient(fdUser, numericReply(server, fdUser,
        "323", RPL_LISTEND));
}

void listSpecificChannel(const int &fdUser, std::vector<std::string> parameter,
    Server *server)
{
    std::vector<std::string> channelList = splitByComma(parameter[0]);
    std::vector<std::string>::iterator itChannelName;
    std::map<std::string, Channel *>::iterator itChannel;

    if (channelList.empty() == true)
        return ;
    for (itChannelName = channelList.begin();
        itChannelName != channelList.end(); itChannelName++)
    {
        itChannel = server->_channelList.find(*itChannelName);
        if (itChannel != server->_channelList.end())
            server->sendClient(fdUser, numericReply(server, fdUser,
                "322", RPL_LIST(itChannel->first, itChannel->second->getTopic())));
    }
}

void list(const int &fdUser, const std::vector<std::string> &parameter,
    const std::string &, Server *server)
{
    // Channel list must exist
    if (server->_channelList.empty() == true)
        return ;
    if (parameter.empty() == true)
        listAllChannel(fdUser, server);
    else
        listSpecificChannel(fdUser, parameter, server);
}
