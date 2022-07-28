#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

void    kill(const int &fd, const std::vector<std::string> &params, \
                       const std::string &,Server *srv)
{
    std::string nickname;
    std::string comment;
    User        *target;

    // param check ----------------------------------------------------------- /
    // check nb of param
    if (params.size() != 2)
    {
        srv->sendClient(fd, \
		numericReply(srv, fd, "461", ERR_NEEDMOREPARAMS(std::string("KILL")))); 
        return;
    }
    nickname = params[0];
    comment = params[1];
    // check if nickname exists
    target = srv->getUserByNickname(nickname);
    if (target == NULL)
    {
        srv->sendClient(fd, \
            numericReply(srv, fd, "401", ERR_NOSUCHNICK(nickname)));
        return;
    }
    // check if user associated with the connection is Op
    if (!target->hasMode(MOD_OPER))
    {
        srv->sendClient(fd, \
            numericReply(srv, fd, "481", ERR_NOPRIVILEGES(nickname)));
        return;
    }

    // all is ok, execute the KILL ------------------------------------------- /
    try { srv->killConnection(target->getFd()); }
    catch (Server::pollDelException &e) { printError(e.what(), 1, true); }
    catch (Server::invalidFdException &e) { printError(e.what(), 1, false); }

    // add the nickname to the list of unavailable nicknames with a timer
    srv->_unavailableNicknames[nickname] = time(NULL);
}
