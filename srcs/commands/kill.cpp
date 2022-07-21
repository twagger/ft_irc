#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

// the server SHOULD add the nickname to the list of unavailable nicknames
// The comment given MUST reflect the actual reason for the KILL.
//     For
//    server-generated KILLs it usually is made up of details concerning
//    the origins of the two conflicting nicknames.

// It is RECOMMENDED that only Operators be allowed to kill other users

// ERR_NOPRIVILEGES > User is not OP 
// ERR_CANTKILLSERVER


std::string kill(int fd, std::vector<std::string> params, Server *srv)
{
    std::string nickname;
    std::string comment;
    User        *target;

    // Param check ----------------------------------------------------------- /
    // check nb of param
    if (params.size() != 2)
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

    // All is ok, execute the kill ------------------------------------------- /
    int temp_fd = 5; // to change when getFd function is available
    //try { srv->killConnection(target->getFd()); }
    try { srv->killConnection(temp_fd); }
    catch (Server::pollDelException &e) { printError(e.what(), 1, true); }
    catch (Server::invalidFdException &e) { printError(e.what(), 1, false); }

    // add the nickname to the list of unavailable nicknames
    srv->_unavailableNicknames.insert(nickname);

    return (NULL);
}
