#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"

void    kill(const int &fd, const std::vector<std::string> &params, \
                       const std::string &,Server *srv)
{
    std::string nickname;
    std::string comment;
    User        *target;

    // COMMAND EXECUTION
    try {
        // check params
        if (params.size() < 2)
            throw needmoreparamsException("KILL");

        nickname = params[0];
        comment = params[1];
        // check if nickname is actually the server hostname
        // This involves that you cannot have a nickname = to server hostname
        if (nickname.compare(srv->getHostname()) == 0)
            throw cantkillserverException();

        // check if nickname exists
        target = srv->getUserByNickname(nickname);
        if (target == NULL)
            throw nosuchnickException(nickname);

        // check if user associated with the connection is Op
        if (!srv->getUserByFd(fd)->hasMode(MOD_OPER))
            throw noprivilegesException();

        // all is ok, execute the KILL
        try { srv->killConnection(target->getFd()); }
        catch (Server::pollDelException &e) { printError(e.what(), 1, true); }
        catch (Server::invalidFdException &e) { printError(e.what(), 1, false); }

        // add the nickname to the list of unavailable nicknames with a timer
        srv->_unavailableNicknames[nickname] = time(NULL);
    }

    // EXCEPTIONS
    catch (noprivilegesException &e) {e.reply(srv, fd); return; }
    catch (nosuchnickException &e) {e.reply(srv, fd); return; }
    catch (needmoreparamsException &e) {e.reply(srv, fd); return; }
    catch (cantkillserverException &e) {e.reply(srv, fd); return; }
}
