#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

/**
 * @brief List all nicknames on a channel
 * 
 * No error handled
 *   
 */

void listUser(const int &fdUser, Server *server,
              std::map<std::string, Channel *>::iterator itChannel)
{
    std::string nicknameList;
    std::deque<User *> listUser = itChannel->second->_users;
    std::deque<User *> listOperator = itChannel->second->_operators;
    std::deque<User *>::iterator itUser;
    std::deque<User *>::iterator itOperator;
    std::string reply;

    for (itUser = listUser.begin(); itUser != listUser.end(); itUser++)
    {
        // Case if user is an operator
        for (itOperator = listOperator.begin(); itOperator != listOperator.end();
             itOperator++)
        {
            if (*itUser == *itOperator)
                nicknameList += "@";
        }
        nicknameList += (*itUser)->getNickname() + " ";
    }
    reply = ":" + server->getHostname() + " " + "353" + " "
						+ server->getUserByFd(fdUser)->getNickname() + " = "
                        + itChannel->second->getChannelName() + " ";
    reply += nicknameList + "\r\n";		
    server->sendClient(fdUser, reply);
}

void listUserInChannel(const int &fdUser, Server *server)
{
    std::string latestChannelJoined;
    std::map<std::string, Channel *>::iterator itChannel;

    if (server->getUserByFd(fdUser)->getChannelsJoined().empty() == true)
        return;
    latestChannelJoined = server->getUserByFd(fdUser)->getChannelsJoined().back();
    itChannel = server->_channelList.find(latestChannelJoined);
    if (itChannel == server->_channelList.end())
        return;
    listUser(fdUser, server, itChannel);
    server->sendClient(fdUser, numericReply(server, fdUser,
                                            "366", RPL_ENDOFNAMES(latestChannelJoined)));
}

void listUserInSpecificChannel(const int &fdUser, const std::vector<std::string> parameter,
                               Server *server)
{
    std::vector<std::string> channelList = splitByComma(parameter[0]);
    std::vector<std::string>::iterator itChannelName;
    std::map<std::string, Channel *>::iterator itChannel;

    if (channelList.empty() == true)
        return;
    for (itChannelName = channelList.begin();
         itChannelName != channelList.end(); itChannelName++)
    {
        itChannel = server->_channelList.find(*itChannelName);
        if (itChannel != server->_channelList.end())
        {
            listUser(fdUser, server, itChannel);
            server->sendClient(fdUser, numericReply(server, fdUser,
                                                    "366", RPL_ENDOFNAMES(*itChannelName)));
        }
    }
}

void names(const int &fdUser, const std::vector<std::string> &parameter,
           const std::string &, Server *server)
{
    if (server->_channelList.empty() == true)
        return;
    if (parameter.empty() == true)
        listUserInChannel(fdUser, server);
    else
        listUserInSpecificChannel(fdUser, parameter, server);
}
