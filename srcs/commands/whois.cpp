#include <algorithm>
#include <sstream>

#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"
#include "../../includes/parser.hpp"

#ifndef SERVERINFO
# define SERVERINFO "No info on this server :("
#endif

/**
 * @brief This command is used to query information about particular user.
 * The server will answer this command with several numeric messages
 * indicating different statuses of each user which matches the mask (if
 * you are entitled to see them).
 * 
 * Errors handled:
 * - ERR_NOSUCHSERVER
 * - ERR_NONICKNAMEGIVEN
 * - ERR_NOSUCHNICK
 *   
 */

static std::string getCommonChannel(Server *srv, const int &fd1, const int &fd2)
{
    std::deque<std::string>                 channelsJoined1;
    std::deque<std::string>                 channelsJoined2;
    std::deque<std::string>::const_reverse_iterator it;
    std::deque<std::string>::const_reverse_iterator itRes;
    User                                    *user1 = srv->getUserByFd(fd1);
    User                                    *user2 = srv->getUserByFd(fd2);

    channelsJoined1 = user1->getChannelsJoined();
    for (it = channelsJoined1.rbegin(); it != channelsJoined1.rend(); ++it)
    {
        channelsJoined2 = user2->getChannelsJoined();
        itRes = std::find(channelsJoined2.rbegin(), channelsJoined2.rend(), \
                          *it);
        if (itRes != channelsJoined2.rend())
            return (*it);
    }
    return (std::string());
}

static bool    isOnTheSameChannel(Server *srv, const int &fd1, const int &fd2)
{ return (!getCommonChannel(srv, fd1, fd2).empty()); }

static void    splitMask(std::deque<std::string> &masks, std::string mask)
{
    // only one mask
    if (mask.find(',') == std::string::npos)
    {
        masks.push_back(mask);
        return;
    }

    // multiple masks   
    while (mask.find(',') != std::string::npos) {
        masks.push_back(mask.substr(0, mask.find(',')));
        mask.erase(0, mask.find(',') + 1);
    }
    masks.push_back(mask);
}

static void    addVisibleUser(Server *srv, const int &fd, \
                       std::deque<User *> &usersList, User *user)
{
    if (isOnTheSameChannel(srv, fd, user->getFd()))
        usersList.push_back(user);
    else if (!user->hasMode(MOD_INVISIBLE))
        usersList.push_back(user);
}

static std::string getChannelList(Server *srv, User *user)
{
    std::deque<std::string>                 chans = user->getChannelsJoined();
    std::deque<std::string>::const_iterator itChan;
    std::deque<User *>                      chanOps;
    std::string                             res;

    for (itChan = chans.begin(); itChan != chans.end(); ++itChan)
    {
        if (itChan != chans.begin())
            res.append(" ");
        chanOps = srv->_channelList[*itChan]->_operators;    
        if (std::find(chanOps.begin(), chanOps.end(), user) != chanOps.end())
            res.append("@");
        res.append(*itChan);
    }
    return (res);
}

// Send information about users visible for me
void whois(const int &fd, const std::vector<std::string> &params, \
         const std::string &, Server *srv)
{
    // Loop and list management
    std::string                             mask;
    std::deque<std::string>                 masks;
    std::deque<std::string>::const_iterator itMasks;
    std::deque<User*>::iterator             it;
    std::deque<User*>                       usersList;
    std::deque<User*>                       allUsers;
    // Infos to send back to requester
    std::stringstream                       ss;
    std::string                             nickname;
    std::string                             username;
    std::string                             realname;
    std::string                             hostname;
    std::string                             servername;
    std::string                             serverinfos;
    std::string                             channelList;
    int                                     seconds;
    bool                                    isOper;

    // COMMAND EXECUTION
    // Params
    try {
        if (params.size() == 0)
            throw nonicknamegivenException();
        if (params.size() > 0)
        {
            if (params.size() > 1)
            {
                if (!matchMask(srv->getHostname().c_str(), params[0].c_str())) {
                    throw nosuchserverException(params[0]);
                }
                mask = params[1];
            }
            else
                mask = params[0];
        }
    }
    // EXCEPTIONS THAT END THE COMMAND
    catch (nonicknamegivenException &e) {e.reply(srv, fd); return; }
    catch (nosuchserverException &e) {e.reply(srv, fd); return; }

    // 1. Split the mask parameter into a deque
    splitMask(masks, mask);

    allUsers = srv->getAllUsers();

    // 2. Add users that fit the mask
    for (itMasks = masks.begin(); itMasks != masks.end(); ++itMasks)
    {
        if ((*itMasks).find('*') == std::string::npos
            && (*itMasks).find('?') == std::string::npos){
            // The mask is a nickname !
            try {
                for (it = allUsers.begin(); it != allUsers.end(); ++it)
                {
                    if ((*it)->getNickname().compare(*itMasks) == 0)
                    {
                        addVisibleUser(srv, fd, usersList, *it);
                        break;
                    }
                }
                if (it == allUsers.end())
                    throw nosuchnickException(*itMasks);
            }
            catch (nosuchnickException &e) {e.reply(srv, fd);}
        }
        else {
            // The mask is ... a mask
            for (it = allUsers.begin(); it != allUsers.end(); ++it)
            {
                // Host
                if ((matchMask((*it)->getHostname().c_str(), (*itMasks).c_str()))
                // Server
                || (matchMask(srv->getHostname().c_str(), (*itMasks).c_str()))
                // Real name
                || (matchMask((*it)->getFullname().c_str(), (*itMasks).c_str()))
                // Nickname
                || (matchMask((*it)->getNickname().c_str(), (*itMasks).c_str())))
                {
                    addVisibleUser(srv, fd, usersList, *it);
                }
            }
        }
    }

    // 3. Loop on the resulting user list and send information about users
    for (it = usersList.begin(); it != usersList.end(); it++)
    {
        // Infos for each user
        nickname = (*it)->getNickname();
        username = (*it)->getUsername();
        realname = (*it)->getFullname();
        hostname = (*it)->getHostname();
        servername = srv->getHostname();
        serverinfos = std::string(SERVERINFO);
        seconds = difftime(time(NULL), (*it)->getLastActivityTime());
        ss << seconds;
        isOper = (*it)->hasMode(MOD_OPER);
        channelList = getChannelList(srv, (*it));

        // Send RPL
        srv->sendClient(fd, numericReply(srv, fd, "311", \
            RPL_WHOISUSER(nickname, username, hostname, realname)));
        srv->sendClient(fd, numericReply(srv, fd, "312", \
            RPL_WHOISSERVER(nickname, servername, serverinfos)));
        srv->sendClient(fd, numericReply(srv, fd, "319", \
            RPL_WHOISCHANNELS(nickname, channelList)));
        if (isOper)
            srv->sendClient(fd, numericReply(srv, fd, "313", \
                RPL_WHOISOPERATOR(nickname)));
        srv->sendClient(fd, numericReply(srv, fd, "317", \
            RPL_WHOISIDLE(nickname, ss.str())));
    }
    srv->sendClient(fd, numericReply(srv, fd, "318", RPL_ENDOFWHOIS(mask)));
}

