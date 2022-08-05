#include <deque>
#include <set>
#include <map>
#include <sstream>

#include "../../includes/commands.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"
#include "../../includes/parser.hpp"

#define MAX_TARGETS 100

/**
 * @brief PRIVMSG is used to send private messages between users, as well as to
 * send messages to channels.  <msgtarget> is usually the nickname of
 * the recipient of the message, or a channel name.
 * 
 * Errors handled:
 * - ERR_NORECIPIENT
 * - ERR_NOTEXTTOSEND
 * - ERR_CANNOTSENDTOCHAN
 * - ERR_WILDTOPLEVEL
 * - ERR_TOOMANYTARGETS
 * - ERR_NOSUCHNICK
 *   
 */

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
// Split the message by comma into multiple targets
static std::map<std::string, std::deque<Target> > splitTargets(std::string targets)
{
    std::map<std::string, std::deque<Target> > map;

    // only one target
    if (targets.find(',') == std::string::npos)
    {
        map[targets] = std::deque<Target>();
        return (map);
    }

    // multiple targets    
    std::string temp;
    std::istringstream stream(targets);
    while (std::getline(stream, temp, ','))
    {
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        map[temp] = std::deque<Target>();
    }
    return (map);
}

static bool    isUserBannedOnChannel(Server *srv, const int &fd, \
                              const std::string &channel)
{
    User                                    *sender;
    std::deque<std::string>                 bannedNicks;
    std::deque<std::string>::const_iterator it;
    std::string                             senderNick;

    sender = srv->getUserByFd(fd);
    if (sender)
    {
        senderNick = sender->getNickname();
        if (srv->_channelList.find(channel) != srv->_channelList.end())
        {
            bannedNicks = srv->_channelList[channel]->_bannedUsers;
            for (it = bannedNicks.begin(); it != bannedNicks.end(); ++it) {
                if (senderNick.compare(*it) == 0) {
                    return (true);
                }
            }
        }
    }
    return (false);
}

/* ************************************************************************** */
/* SPECIFIC FUNCTIONS TO EXTRACT USER FD, CHANNEL NAME OR TO COMPUTE MASK     */
/* ************************************************************************** */
// CHANNEL
static const std::string extractChannelName(const std::string str, Server *srv)
{
    int         pos = 0;
    std::string name;

    if (str[0] == '!') {
        // CHANNEL ID : we don't use it so we can just skip it
        for (int i = 0; i < 5; ++i)
        {
            if (!std::isdigit(str[i]))
                throw nosuchnickException(str);
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
static int   extractUserFd(const std::string str, Server *srv)
{
    std::string user;
    std::string host;
    std::string servername;
    std::string nickname;
    User        *resultUser = NULL;

    // string matching using
    if (str.find('%') != std::string::npos) {
        // USER % HOST
        user = str.substr(0, str.find('%'));
        host = str.substr(str.find('%') + 1);
        if (host.find('@') != std::string::npos) {
            // USER % HOST @ SERVERNAME
            servername = host.substr(host.find('@') + 1);
            host.erase(host.find('@'));
        }
    }
    else if (str.find('!') != std::string::npos) {
        // NICKNAME ! USER @ HOST
        nickname = str.substr(0, str.find('!'));
        user = str.substr(str.find('!') + 1);
        if (user.find('@') == std::string::npos)
            throw nosuchnickException(str);
        host = user.substr(user.find('@') + 1);
        user.erase(user.find('@'));
    }
    else if (str.find('@') != std::string::npos) {
        // USER @ SERVERNAME
        user = str.substr(0, str.find('@'));
        servername = str.substr(str.find('@') + 1);
    }
    else
        nickname = str;

    // User search and return
    if (!nickname.empty())
    {
        resultUser = srv->getUserByNickname(nickname);
        if (resultUser == NULL)
            throw nosuchnickException(str);
    }
    else if (!user.empty() && !host.empty())
    {
        resultUser = srv->getUserByUsername(user, host);
        if (resultUser == NULL)
            throw nosuchnickException(str);
    }
    else if (!user.empty())
    {
        resultUser = srv->getUserByUsername(user);
        if (resultUser == NULL)
            throw nosuchnickException(str);
    }

    return (resultUser->getFd());
}

// MASK
static void    computeMask(const int &fd, const std::string &str, Server *srv, \
                    std::deque<Target> &target)
{
    std::deque<User*>                   userList;
    std::deque<User*>::const_iterator   it;
    std::string                         mask;
    size_t                              toplevel;
    
    // Check user privileges : must be oper to use mask
    if (!srv->getUserByFd(fd)->hasMode(MOD_OPER))
        throw noprivilegesException();

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
static void getTargetsFromString(const int &fd, const std::string &str, \
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
        try { computeMask(fd, str, srv, target); }
        catch (noprivilegesException &e) { e.reply(srv, fd); }  
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
    std::map<std::string, std::deque<Target> >              targets;
    std::map<std::string, std::deque<Target> >::iterator    it;
    std::string                                             msgtarget;
    std::string                                             message;
    std::deque<Target>                                      target;
    std::deque<Target>::iterator                            itTarg;
    std::stringstream                                       ss;

    // COMMAND EXECUTION
    try {
        // check nb of param
        if (params.size() == 0)
            throw norecipientException("PRIVMSG");
        if (params.size() == 1)
            throw notexttosendException();

        msgtarget = params[0];
        message = params[1];
        
        // Split targets into a map
        targets = splitTargets(msgtarget);
        
        // Too many targets ?
        if (targets.size() > MAX_TARGETS)
            throw toomanytargetsException(msgtarget, ss.str(), "Aborted.");
        
    }
    // EXCEPTIONS THAT END THE COMMAND
    catch (norecipientException &e) {e.reply(srv, fd); return; }
    catch (notexttosendException &e) {e.reply(srv, fd); return; }
    catch (toomanytargetsException &e) {e.reply(srv, fd); return; }
    
    // Loop to SEND all messages
    for (it = targets.begin(); it != targets.end(); ++it)
    {
        try {
            getTargetsFromString(fd, it->first, it->second, srv);
            target = it->second;
            for (itTarg = target.begin(); itTarg != target.end(); ++itTarg)
            {
                // Check target validity here instead of upper
                if (itTarg->fd != -1) {
                    // Send to user (we don't handle AWAY flag on user)
                    srv->sendClient(itTarg->fd, \
                        clientReply(srv, fd, PRIVMSG(itTarg->target, message)));
                }
                else if (!itTarg->channel.empty()) {
                    // Send to channel : no exception cannot send because 
                    //  we do not handle the needed chan mode for this
                    if (isUserBannedOnChannel(srv, fd, itTarg->channel))
                        throw cannotsendtochanException(itTarg->channel);
                    srv->sendChannel(itTarg->channel, clientReply(srv, fd, \
                                     PRIVMSG(itTarg->target, message)), fd);
                }
            }
        }
        // EXCEPTIONS THAT DON'T END THE COMMAND
        catch (nosuchnickException &e) {e.reply(srv, fd);}
        catch (notoplevelException &e) {e.reply(srv, fd);}
        catch (wildtoplevelException &e) {e.reply(srv, fd);}
        catch (cannotsendtochanException &e) {e.reply(srv, fd);}
    }
}
