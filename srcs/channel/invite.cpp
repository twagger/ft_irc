#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

int checkParameterInvite(std::string nickname, std::string channel, const int &fdUser, Server *server)
{
    // Nickname and channel must not be empty
    if (nickname.empty() == true || channel.empty() == true)
        return (-1);
    // User must exist
    if (server->getUserByNickname(nickname) == NULL)
        return (-2);
    // User is already on channel
    std::map<std::string, Channel *>::iterator it = server->_channelList.find(channel);
    if (findUserOnChannel(it->second->_users, server->getUserByNickname(nickname)) == it->second->_users.end())
        return (-3);
    // Current user must be in channel
    if (findUserOnChannel(it->second->_users, server->getUserByFd(fdUser)) == it->second->_users.end())
        return (-4);
    // Current user must be an operator if channel is invite only
    std::vector<char>::iterator itMode = it->second->_mode.find('i');
    if (itMode != it->second->_mode.end()
            && findUserOnChannel(it->second->_operators, server->getUserByFd(fdUser)) == it->second->_operators.end())
        return (-5);      
}

// Que se passe-t-il si nous invitons un utilisateur dans un channel qui n'existe pas ? Il devient operateur de ce channel ainsi que l'utilisateur qui invite ?

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
        return (numericReply(server, fdUser, "443", ERR_USERONCHANNEL(userToInvite->getFullname(), channel)));
    if (checkParameterInvite(nickname, channel, fdUser, server) == -4)
        return (numericReply(server, fdUser, "442", ERR_NOTONCHANNEL(channel)));
    if (checkParameterInvite(nickname, channel, fdUser, server) == -5)
        return (numericReply(server, fdUser, "482", ERR_CHANOPRIVSNEEDED(channel)));
}