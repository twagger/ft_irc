#include <deque>
#include <set>
#include <sstream>

#include "../../includes/commands.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"
#include "../../includes/parser.hpp"

#define MAX_TARGETS 100

/* ************************************************************************** */
/* STRUCTURE TO HOST CLIENT FD OR CHANNEL NAME AND TARGET OF THE MESSAGE      */
/* ************************************************************************** */
struct Target {
    int         fd;
    std::string channel;
    std::string target;

    Target(int fd, std::string target, std::string channel = std::string())
    : fd(fd), channel(channel), target(target) {}
};

/* ************************************************************************** */
/* UTILITY FUNCTIONS                                                          */
/* ************************************************************************** */
void    checkChannelRights(Server *srv, const int &fd, \
                                                    const std::string channel)
{
    std::map<std::string, Channel *>::const_iterator    itChan;
    std::deque<User*>                                   userList;
    std::deque<User*>                                   banList;
    std::deque<User*>::const_iterator                   it;
    bool                                                result;

    // Get the list of users in the channel
    itChan = srv->_channelList.find(channel);
    if (itChan == srv->_channelList.end())
        throw nosuchnickException(channel);

    // Get lists
    userList = itChan->second->_users;
    banList = itChan->second->_bannedUsers;

    // Check the user fd against users of the channel then banlist
    result = false;
    // Check user list
    for (it = userList.begin(); it != userList.end(); ++it)
    {
        if ((*it)->getFd() == fd)
        {
            result = true;
            break;
        }
    }
    if (result == false)
        throw cannotsendtochanException(channel);
    else {
        // Check banlist
        for (it = banList.begin(); it != banList.end(); ++it)
        {
            if ((*it)->getFd() == fd)
            {
                result = false;
                break;
            }
        }
    }
    if (result == false)
        throw cannotsendtochanException(channel);
}

void    paramGrammarCheck(const std::string user, const std::string host, \
                       const std::string servername, const std::string nickname)
{
    // For basic grammar checking
    std::string nickControl("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN\
                             OPQRSTUVWXYZ-[]\\`_^{|}");
    std::string userControl("\0\r\n @");
    std::string hostControl("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN\
                             OPQRSTUVWXYZ-.:");
    std::string serverControl("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKL\
                               MNOPQRSTUVWXYZ-.");
    
    // Basic grammar check
    if (!nickname.empty())
    {
        if (nickname.find_first_not_of(nickControl) != std::string::npos)
            throw grammarException("Grammar : nickname");
        if (nickname.length() < 1 || nickname.length() > 9)
            throw grammarException("Grammar : nickname");
        if (nickname[0] == '-' || std::isdigit(nickname[0]))
            throw grammarException("Grammar : nickname");
    }
    if (!user.empty())
    {
        if (user.find_first_of(userControl) != std::string::npos)
            throw grammarException("Grammar : user");
        if (user.length() < 1)
            throw grammarException("Grammar : user");
    }
    if (!servername.empty())
    {
        if (servername.find_first_not_of(serverControl) != std::string::npos)
            throw grammarException("Grammar : servername");
    }
    if (!host.empty())
    {
        if (host.find_first_not_of(hostControl) != std::string::npos)
            throw grammarException("Grammar : hostname");
    }
}

void cleanTargetsList(std::deque<Target> &target)
{
    std::deque<Target>::iterator    it;
    std::set<int>                   alreadyFds;
    std::set<std::string>           alreadyChannels;

    // Clean target fds or channel that appears multiple time
    for (it = target.begin(); it != target.end();)
    {
        if (it->fd != -1)
        {
            if (alreadyFds.find(it->fd) == alreadyFds.end())
            {
                alreadyFds.insert(it->fd);
                ++it;
            }
            else
                target.erase(it++);
        }
        else if (!(it->channel.empty()))
        {
            if (alreadyChannels.find(it->channel) == alreadyChannels.end())
            {
                alreadyChannels.insert(it->channel);
                ++it;
            }
            else
                target.erase(it++);
        }
        else
            ++it;
    }
}

/* ************************************************************************** */
/* SPECIFIC FUNCTIONS TO EXTRACT USER FD, CHANNEL NAME OR TO COMPUTE MASK     */
/* ************************************************************************** */
// CHANNEL
const std::string extractChannelName(const std::string str, Server *srv)
{
    int         pos = 0;
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

    // Param check
    paramGrammarCheck(user, host, servername, nickname);

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
void    computeMask(const std::string &str, Server *srv, \
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
    else if (mask.find('*', toplevel) != std::string::npos)
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
void getTargetsFromString(const std::string &str, \
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
    std::vector<std::string>                    targets;
    std::vector<std::string>::const_iterator    it;
    std::string                                 msgtarget;
    std::string                                 message;
    std::deque<Target>                          target;
    std::deque<Target>::iterator                itg;
    std::stringstream                           ss;
    int                                         nbTargets = 0;

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

        // Clean the targets list from doubles
        cleanTargetsList(target);

        // Second loop TO SEND all messages
        for (itg = target.begin(); itg != target.end(); ++itg)
        {
            // HANDLE HERE : RPL_AWAY, ERR_CANNOTSENDTOCHAN
            if (itg->fd != -1) {
                // Send to user
                srv->sendClient(itg->fd, \
                        clientReply(srv, fd, PRIVMSG(itg->target, message)));
            }
            else if (!itg->channel.empty()) {
                // Send to channel
                checkChannelRights(srv, fd, itg->channel);
                srv->sendChannel(itg->channel, \
                        clientReply(srv, fd, PRIVMSG(itg->target, message)), fd);
            }
        }
    } // EXCEPTIONS
    catch (grammarException &e) { printError(e.what(), 1, false); return; }
    catch (norecipientException &e) {e.reply(srv, fd); return; }
    catch (nosuchnickException &e) {e.reply(srv, fd); return; }
    catch (notexttosendException &e) {e.reply(srv, fd); return; }
    catch (toomanytargetsException &e) {e.reply(srv, fd); return; }
    catch (notoplevelException &e) {e.reply(srv, fd); return; }
    catch (wildtoplevelException &e) {e.reply(srv, fd); return; }
    catch (cannotsendtochanException &e) {e.reply(srv, fd); return; }
}
