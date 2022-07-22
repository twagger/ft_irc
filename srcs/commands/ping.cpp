#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

std::string ping(int fd, std::vector<std::string> params, Server *srv)
{
     std::string nickname;
    std::string comment;
    User        *target;

    // Param check ----------------------------------------------------------- /
    // check nb of param
    if (params.size() != 1)
        return (reply(srv, fd, "461", ERR_NEEDMOREPARAMS(std::string("KILL")))); 
    nickname = params[0];
    comment = params[1];
    // check if nickname exists
    target = srv->getUserByNickname(nickname);
    if (target == NULL)
        return (reply(srv, fd, "401", ERR_NOSUCHNICK(nickname)));
    // check if user associated with the connection is Op
    if (target->getMode() != 1)
        return (reply(srv, fd, "481", ERR_NOPRIVILEGES(nickname)));
    return (NULL);
}
