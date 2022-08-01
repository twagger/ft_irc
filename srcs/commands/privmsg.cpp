#include <deque>
#include <sstream>

#include "../../includes/commands.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"

#define MAX_TARGETS 100

/* ************************************************************************** */
/* STRUCTURE TO HOST CLIENT FD OR CHANNEL NAME                                */
/* ************************************************************************** */
struct Target {
    int         fd;
    std::string name;

    Target(int fd, std::string name = std::string())
    : fd(fd), name(name) {}
    Target  &operator=(const Target &rhs) 
    { this->fd = rhs.fd; this->name = rhs.name; }
};

/* ************************************************************************** */
/* SPECIFIC FUNCTIONS TO EXTRACT USER FD, CHANNEL NAME OR TO COMPUTE MASK     */
/* ************************************************************************** */
const std::string extractChannelName(const std::string str, Server *srv)
{
    int         pos = 1;
    std::string name;

    if (str[0] == '!') {
        // CHANNEL ID : we don't use it so we can just skip it
        for (int i = 0; i < 5; ++i)
        {
            if (!std::isdigit(str[i]))
                throw grammarException();
        }
        pos = 6;
    }
    if (str.find(':') != std::string::npos)
        name = str.substr(pos, str.find(':') - pos);
    else
        name = str.substr(pos, std::string::npos);
    return (name);
}

//ERR_NOSUCHNICK
const int   extractUserFd(const std::string str, Server *srv)
{
    std::string user;
    std::string host;
    std::string servername;
    std::string nickname;
    User        *resultUser;

    // For basic grammar checking
    std::string nickControl("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN\
                             OPQRSTUVWXYZ-[]\\`_^{|}");
    std::string userControl("\0\r\n @");
    std::string hostControl("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN\
                             OPQRSTUVWXYZ-.:");
    std::string serverControl("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKL\
                               MNOPQRSTUVWXYZ-.");

    // string matching using
    if (str.find('%') != std::string::npos) {
        // USER / HOST / SERVERNAME
        user = str.substr(0, str.find('%') - 1);
        host = str.substr(str.find('%') + 1);
        if (host.find('@')) {
            servername = host.substr(host.find('@') + 1);
            host.erase(host.find('@'));
        }
    }
    else if (str.find('!') != std::string::npos) {
        // NICKNAME
        nickname = str.substr(0, str.find('!') - 1);
        user = str.substr(str.find('!') + 1);
        if (user.find('@') == std::string::npos)
            throw grammarException();
        host = user.substr(user.find('@') + 1);
        user.erase(user.find('@'));
    }
    else
        nickname = str;

    // Basic grammar check
    if (nickname.find_first_not_of(nickControl) != std::string::npos)
        throw grammarException();
    if (nickname.length() < 1 || nickname.length() > 9)
        throw grammarException();
    if (nickname[0] == '-' || std::isdigit(nickname[0]))
        throw grammarException();
    if (user.find_first_of(userControl) != std::string::npos)
        throw grammarException();
    if (user.length() < 1)
        throw grammarException();
    if (servername.find_first_not_of(serverControl) != std::string::npos)
        throw grammarException();
    if (host.find_first_not_of(hostControl) != std::string::npos)
        throw grammarException();

    // User search and return
    if (!nickname.empty())
    {
        resultUser = srv->getUserByNickname(nickname);
        if (resultUser == NULL)
            throw nosuchnickException(user);
    }
    else if (!user.empty() && !host.empty())
    {
        resultUser = srv->getUserByUsername(user, host);
        if (resultUser == NULL)
            throw nosuchnickException(user.append("%").append(host));
    }
    else if (!user.empty())
    {
        resultUser = srv->getUserByUsername(user);
        if (resultUser == NULL)
            throw nosuchnickException(user);
    }

    return (resultUser->getFd());
}

// ERR_NOTOPLEVEL, ERR_WILDTOPLEVEL,
// # : HOSTMASK | $ : SERVERMASK for users only : the mask must have one . in it
void    computeMask(const std::string str, Server *srv, \
                    std::deque<Target> &target)
{
    if (str[0] == '$') {
        // Server mask

    }
    else {
        // Host mask
    }
}

/* ************************************************************************** */
/* SPECIFIC FUNCTIONS TO CREATE A COLLECTION OF TARGETS FOR THE MAIN FUNCTION */
/* ************************************************************************** */
void getTargetsFromString(const std::string str, \
                          std::deque<Target> &target, Server *srv)
{
    if (str[0] == '+' || str[0] == '&' || str[0] == '!'
        || (str[0] == '#' && str.find('.') == std::string::npos)) {
        // CHANNEL
        target.push_back(Target(-1, extractChannelName(str, srv)));
    }
    else if (str[0] == '$' 
             || (str[0] == '#' && str.find('.') != std::string::npos)) {
        // MASK
        computeMask(str, srv, target);
    }
    else {
        // USER
        target.push_back(Target(extractUserFd(str, srv)));
    }
}

/* ************************************************************************** */
/* IRC COMMAND : PRIVMSG                                                      */
/* ************************************************************************** */
void privmsg(const int &fd, const std::vector<std::string> &params, \
                        const std::string &, Server *srv)
{
    std::vector<std::string>            targets;
    std::vector<std::string>::iterator  it;
    std::string                         msgtarget;
    std::string                         text;
    std::deque<Target>                  target;
    std::deque<Target>::iterator        itg;
    std::stringstream                   ss;
    int                                 nbTargets = 0;

    // COMMAND EXECUTION
    try
    {
        // check nb of param
        if (params.size() == 0)
            throw norecipientException("PRIVMSG");
        if (params.size() == 1)
            throw notexttosendException();
        
        // Two params, iterate throught the first to and send
        msgtarget = params[0];
        text = params[1];
        targets = splitByComma(msgtarget);
        // First loop WITH NO SEND to count the exact number of targets
        for (it = targets.begin(); it != targets.end(); ++it)
        {
            getTargetsFromString(*it, target, srv);
            nbTargets += target.size();
        }
        ss << nbTargets; 
        if (nbTargets > MAX_TARGETS)
            throw toomanytargetsException(msgtarget, ss.str(), "Aborted.");

        // Second loop TO SEND all messages
        for (itg = target.begin(); itg != target.end(); ++it)
        {
            // HANDLE HERE : RPL_AWAY, ERR_CANNOTSENDTOCHAN
            if (itg->fd != -1) {
                // Send to user
            }
            else if (!itg->name.empty()) {
                // Send to channel
            }
        }
    } // EXCEPTIONS
    catch (grammarException &e) { printError(e.what(), 1, false); return; }
    catch (norecipientException &e) {e.reply(srv, fd); return; }
    catch (notexttosendException &e) {e.reply(srv, fd); return; }
    catch (toomanytargetsException &e) {e.reply(srv, fd); return; }
}
