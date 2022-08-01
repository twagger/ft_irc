#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

int checkParameterInvite(std::string nickname, std::string channel,
                         const int &fdUser, Server *server, User *userToInvite)
{
    // Nickname and channel must not be empty
    if (nickname.empty() == true || channel.empty() == true)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "461", ERR_NEEDMOREPARAMS(std::string("INVITE"))));
        return (-1);
    }
    // Channel list must not be empty
    if (server->_channelList.empty() == true)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "403", ERR_NOSUCHCHANNEL(channel)));
        return (-1);
    }
    // User must exist
    if (server->getUserByNickname(nickname) == NULL)
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "401", ERR_NOSUCHNICK(nickname)));
        return (-2);
    }
    // According to RFC "There is no requirement that the
    // channel the target user is being invited to must exist or be a valid
    // channel". We chose to have an existing channel as a requirement
    std::map<std::string, Channel *>::iterator it = server->_channelList.find(channel);
    if (it == server->_channelList.end())
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "403", ERR_NOSUCHCHANNEL(channel)));
        return (-3);
    }
    // User is already on channel
    if (findUserOnChannel(it->second->_users, server->getUserByNickname(nickname)) == it->second->_users.end())
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "443", ERR_USERONCHANNEL(userToInvite->getFullname(), channel)));
        return (-4);
    }
    // Current user must be in channel
    if (findUserOnChannel(it->second->_users,
                          server->getUserByFd(fdUser)) == it->second->_users.end())
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "442", ERR_NOTONCHANNEL(channel)));
        return (-5);
    }
    // Current user must be an operator if channel is invite only
    if (it->second->hasMode(MOD_INVITE) && findUserOnChannel(it->second->_operators,
                                                               server->getUserByFd(fdUser)) == it->second->_operators.end())
    {
        server->sendClient(fdUser, numericReply(server, fdUser,
                                                "482", ERR_CHANOPRIVSNEEDED(channel)));
        return (-6);
    }
    return (0);
}

void invite(const int &fdUser, const std::vector<std::string> &parameter, const std::string &, Server *server)
{
    std::string nickname = parameter[0];
    std::string channel = parameter[1];
    User *userToInvite = server->getUserByNickname(nickname);

    if (checkParameterInvite(nickname, channel, fdUser, server, userToInvite) < 0)
        return;
    // Add user to the list of invitee and return reply
    std::map<std::string, Channel *>::iterator channelPos = server->_channelList.find(channel);
    channelPos->second->addUser(server->getUserByNickname(nickname));
    server->sendClient(fdUser, clientReply(server, fdUser, "INVITE " +
            server->getUserByNickname(nickname)->getNickname()) + " " + channel);
    server->sendClient(server->getUserByNickname(nickname)->getFd(),
        clientReply(server, fdUser, "INVITE " +
            server->getUserByNickname(nickname)->getNickname()) + " " + channel);
}
