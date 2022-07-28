#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

int checkParameterInvite(std::string nickname, std::string channel, const int &fdUser, Server *server)
{
    // Channel list must not be empty
    if (server->_channelList.empty() == true)
        return (-1);
    // Nickname and channel must not be empty
    if (nickname.empty() == true || channel.empty() == true)
        return (-1);
    // User must exist
    if (server->getUserByNickname(nickname) == NULL)
        return (-2);
    // According to RFC "There is no requirement that the 
    // channel the target user is being invited to must exist or be a valid
    // channel". We chose to have an existing channel as a requirement
    std::map<std::string, Channel *>::iterator it = server->_channelList.find(channel);
    if (it == server->_channelList.end())
        return (-3);
    // User is already on channel
    if (findUserOnChannel(it->second->_users, server->getUserByNickname(nickname)) == it->second->_users.end())
        return (-4);
    // Current user must be in channel
    if (findUserOnChannel(it->second->_users, server->getUserByFd(fdUser)) == it->second->_users.end())
        return (-5);
    // Current user must be an operator if channel is invite only
    std::vector<char>::iterator itMode = findMode(it->second->_mode, 'i');
    if (itMode != it->second->_mode.end()
            && findUserOnChannel(it->second->_operators, server->getUserByFd(fdUser)) == it->second->_operators.end())
        return (-6);
    return (0);   
}

const std::string invite(const int &fdUser, const std::vector<std::string> &parameter, Server *server)
{
    std::string nickname = parameter[0];
    std::string channel = parameter[1];
    User *userToInvite = server->getUserByNickname(nickname);

    if (checkParameterInvite(nickname, channel, fdUser, server) == -1)
        return (numericReply(server, fdUser, "461", ERR_NEEDMOREPARAMS(std::string("INVITE"))));
    if (checkParameterInvite(nickname, channel, fdUser, server) == -2)
        return (numericReply(server, fdUser, "401", ERR_NOSUCHNICK(nickname)));
    if (checkParameterInvite(nickname, channel, fdUser, server) == -3)
        return (numericReply(server, fdUser, "403", ERR_NOSUCHCHANNEL(channel)));
    if (checkParameterInvite(nickname, channel, fdUser, server) == -4)
        return (numericReply(server, fdUser, "443", ERR_USERONCHANNEL(userToInvite->getFullname(), channel)));
    if (checkParameterInvite(nickname, channel, fdUser, server) == -5)
        return (numericReply(server, fdUser, "442", ERR_NOTONCHANNEL(channel)));
    if (checkParameterInvite(nickname, channel, fdUser, server) == -6)
        return (numericReply(server, fdUser, "482", ERR_CHANOPRIVSNEEDED(channel)));
    
    // Add user to the list of invitee and return reply
    std::map<std::string, Channel *>::iterator channelPos = server->_channelList.find(channel);
    channelPos->second->addUser(server->getUserByNickname(nickname));
    return (clientReply(server, fdUser, nickname + " has been invited to " +
        channel + "by " + server->getUserByFd(fdUser)->getNickname()));
}
