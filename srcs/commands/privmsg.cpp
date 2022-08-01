#include <deque>
#include <sstream>

#include "../../includes/commands.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"
#include "../../includes/parser.hpp"

#define MAX_TARGETS 100

/* ************************************************************************** */
/* STRUCTURE TO HOST CLIENT FD OR CHANNEL NAME                                */
/* ************************************************************************** */
struct Target {
    int         fd;
    std::string channel;
    std::string target;

    Target(int fd, std::string target, std::string channel = std::string())
    : fd(fd), channel(channel), target(target) {}
};

/* ************************************************************************** */
/* SPECIFIC FUNCTIONS TO EXTRACT USER FD, CHANNEL NAME OR TO COMPUTE MASK     */
/* ************************************************************************** */
// CHANNEL
const std::string extractChannelName(const std::string str, Server *srv)
{
    int         pos = 1;
    std::string name;

    if (str[0] == '!') {
        // CHANNEL ID : we don't use it so we can just skip it
        for (int i = 0; i < 5; ++i)
        {
            if (!std::isdigit(str[i]))
                throw grammarException("Grammar : channel name");
        }
        pos = 6;
    }
    if (str.find(':') != std::string::npos)
        name = str.substr(pos, str.find(':') - pos);
    else
        name = str.substr(pos, std::string::npos);
    
    // Check if channel exists
    if (srv->_channelList.find(name) == srv->_channelList.end())
        throw nosuchnickException(name);
    return (name);
}

// USER
int   extractUserFd(const std::string str, Server *srv)
{
    std::string user;
    std::string host;
    std::string servername;
    std::string nickname;
    User        *resultUser = NULL;

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
            throw grammarException("Grammar : nickname");
        host = user.substr(user.find('@') + 1);
        user.erase(user.find('@'));
    }
    else
        nickname = str;

    // Basic grammar check
    // > ne check que ce qui est rempli
    if (nickname.find_first_not_of(nickControl) != std::string::npos)
        throw grammarException("Grammar : nickname");
    if (nickname.length() < 1 || nickname.length() > 9)
        throw grammarException("Grammar : nickname");
    if (nickname[0] == '-' || std::isdigit(nickname[0]))
        throw grammarException("Grammar : nickname");
    if (user.find_first_of(userControl) != std::string::npos)
        throw grammarException("Grammar : user");
    if (user.length() < 1)
        throw grammarException("Grammar : user");
    if (servername.find_first_not_of(serverControl) != std::string::npos)
        throw grammarException("Grammar : servername");
    if (host.find_first_not_of(hostControl) != std::string::npos)
        throw grammarException("Grammar : hostname");

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

// MASK
void    computeMask(const std::string str, Server *srv, \
                    std::deque<Target> &target)
{
    std::deque<User*>                   userList;
    std::deque<User*>::const_iterator   it;
    std::string                         mask;
    size_t                              toplevel;
    
    // Check the mask for exceptions
    mask = str.substr(1);
    toplevel = mask.find_last_of('.');
    if (toplevel == std::string::npos)
        throw notoplevelException(mask);
    else if (mask.find('*', toplevel))
        throw wildtoplevelException(mask);

    if (str[0] == '$') {
        // Server mask
        if (matchMask(srv->getHostname().c_str(), \
                      str.substr(1).c_str()) == true) {
            userList = srv->getAllUsers();
            for (it = userList.begin(); it != userList.end(); ++it)
                target.push_back(Target((*it)->getFd(), str));
        }
    }
    else {
        // Host mask
        userList = srv->getAllUsers();
        for (it = userList.begin(); it != userList.end(); ++it)
        {
            if (matchMask((*it)->getHostname().c_str(), \
                          str.substr(1).c_str()) == true)
                target.push_back(Target((*it)->getFd(), str));
        }
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
        target.push_back(Target(-1, str, extractChannelName(str, srv)));
    }
    else if (str[0] == '$' 
             || (str[0] == '#' && str.find('.') != std::string::npos)) {
        // MASK
        computeMask(str, srv, target);
    }
    else {
        // USER
        target.push_back(Target(extractUserFd(str, srv), str));
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
    std::string                         message;
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
        message = params[1];
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
                srv->sendClient(itg->fd, \
                        clientReply(srv, fd, PRIVMSG(itg->target, message)));
            }
            else if (!itg->channel.empty()) {
                // Send to channel
                srv->sendChannel(itg->channel, \
                        clientReply(srv, fd, PRIVMSG(itg->target, message)));
            }
        }
    } // EXCEPTIONS
    catch (grammarException &e) { printError(e.what(), 1, false); return; }
    catch (norecipientException &e) {e.reply(srv, fd); return; }
    catch (notexttosendException &e) {e.reply(srv, fd); return; }
    catch (toomanytargetsException &e) {e.reply(srv, fd); return; }
    catch (notoplevelException &e) {e.reply(srv, fd); return; }
    catch (wildtoplevelException &e) {e.reply(srv, fd); return; }
}
