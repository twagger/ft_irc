#include "../../includes/channel.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

int checkParameter(std::vector<std::string> channel, std::vector<std::string> user, std::string kickMessage,
    Server *server, const int &fdUser)
{
    // user and channel must not be empty
    if (user.empty() == true || channel.empty() == true)
        return (-1);

}

const std::string kick(const int &fdUser, const std::vector<std::string> &parameter, Server *server)
{
    std::vector<std::string> channel;
    std::vector<std::string> user;
    std::string kickMessage;

    channel = splitByComma(parameter[0]);
    if (parameter.size() > 1)
        user = splitByComma(parameter[1]);
    if (parameter.size() > 2)
        kickMessage = parameter[3];
    if (checkParameter(channel, user, server, fdUser) == -1)
        return (numericReply(server, fdUser, "461", ERR_NEEDMOREPARAMS(std::string("KICK"))));

    if (channel.size() == 1)
        return (oneChannelCase(channel, user, kickMessage, server, fdUser));
    else
        return (multipleChannelCase(channel, user, kickMessage, server, fdUser));
    return (NULL);   
}