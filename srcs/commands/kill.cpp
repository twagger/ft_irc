#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

std::string kill(const int fd, std::vector<std::string> params, Server *srv)
{
    std::string nickname;
    std::string comment;
    User        *target;

    // param check ----------------------------------------------------------- /
    // check nb of param
    if (params.size() != 2)
        return (numericReply(srv, fd, "461", ERR_NEEDMOREPARAMS(std::string("KILL")))); 
    nickname = params[0];
    comment = params[1];
    // check if nickname exists
    target = srv->getUserByNickname(nickname);
    if (target == NULL)
        return (numericReply(srv, fd, "401", ERR_NOSUCHNICK(nickname)));
    // check if user associated with the connection is Op
    if (target->getMode() != 1)
        return (numericReply(srv, fd, "481", ERR_NOPRIVILEGES(nickname)));

    // all is ok, execute the KILL ------------------------------------------- /
    try { srv->killConnection(target->getFd()); }
    catch (Server::pollDelException &e) { printError(e.what(), 1, true); }
    catch (Server::invalidFdException &e) { printError(e.what(), 1, false); }

    // add the nickname to the list of unavailable nicknames
    srv->_unavailableNicknames.insert(nickname);

    // no specific reply on success
    return (NULL);
}
