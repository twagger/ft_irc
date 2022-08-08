#include <algorithm>

#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"
#include "../../includes/parser.hpp"

/**
 * @brief The WHO command is used by a client to generate a query which returns
 * a list of information which 'matches' the <mask> parameter given by
 * the client.
 * 
 * Errors handled:
 * - ERR_NOSUCHSERVER
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

static bool    isUserChanOper(Server *srv, User *user, const std::string channel)
{
    std::deque<User *>                  userList;
    std::deque<User *>::const_iterator  it;

    if (channel.empty())
        return (false);
    userList = srv->_channelList[channel]->_operators;
    if (std::find(userList.begin(), userList.end(), user) == userList.end())
        return (false);
    return (true);
}

// Send information about users visible for me
void who(const int &fd, const std::vector<std::string> &params, \
         const std::string &, Server *srv)
{
    std::string                 mask;
    std::string                 name;
    std::string                 channel;
    bool                        onlyOpers = false;
    std::deque<User*>           usersList;
    std::deque<User*>           allUsers;
    std::deque<User*>::iterator it;
    std::deque<std::string>     channelJoined;

    // COMMAND EXECUTION
    // Params
    if (params.size() > 0)
    {
        mask = params[0];
        if (params.size() > 1 and params[1].compare("o") == 0)
            onlyOpers = true;
    }

    // 1. All visible users : same channel or non "i"
    allUsers = srv->getAllUsers();
    for (it = allUsers.begin(); it != allUsers.end(); it++)
    {
        // Check if user is non invisible
        if ((*it)->hasMode(MOD_INVISIBLE) == false || (*it)->getFd() == fd) 
            usersList.push_back(*it);
        else if (isOnTheSameChannel(srv, fd, (*it)->getFd()) == true)
            usersList.push_back(*it);
    }

    if (srv->_channelList.find(mask) != srv->_channelList.end()) {
        // 2. Is the match a channel ? Erase all users not in that channel
        for (it = usersList.begin(); it != usersList.end();)
        {
            channelJoined = (*it)->getChannelsJoined();
            if (std::find(channelJoined.begin(), channelJoined.end(), mask) \
                    == channelJoined.end())
            {
                it = usersList.erase(it);
            }
            else
                ++it;
        }
        // Set the channel in the results to be to channel asked
        channel = mask;
    }
    else {
        // 3. The mask is not a channel : Check the mask against users's Host, 
        // Server, Real name, Nickname
        for (it = usersList.begin(); it != usersList.end();)
        {
            // Host
            if ((matchMask((*it)->getHostname().c_str(), mask.c_str()) == true)
            // Server
            || (matchMask(srv->getHostname().c_str(), mask.c_str()) == true)
            // Real name
            || (matchMask((*it)->getFullname().c_str(), mask.c_str()) == true)
            // Nickname
            || (matchMask((*it)->getNickname().c_str(), mask.c_str()) == true))
            {
                ++it;
                continue;
            }
            else
                it = usersList.erase(it);
        }
    }

    if (onlyOpers == true) {
        // 4. If "o" option is set, apply oper option on the list
        for (it = usersList.begin(); it != usersList.end();)
        {
            // If user hasMode(MOD_OPER) == false > Remove
            if ((*it)->hasMode(MOD_OPER) == false)
                it = usersList.erase(it);
            else
                ++it;
        }
    }

    // 5. Loop on the resulting user list and send information about users
    for (it = usersList.begin(); it != usersList.end(); it++)
    {
        if (channel.empty())
            channel = getCommonChannel(srv, fd, (*it)->getFd());
        srv->sendClient(fd, \
           numericReply(srv, fd, "352", RPL_WHOREPLY(\
            channel, \
            (*it)->getUsername(), \
            (*it)->getHostname(), \
            srv->getHostname(), \
            (*it)->getNickname(), \
            std::string("H"), \
            ( (*it)->hasMode(MOD_OPER) ? std::string("*") : std::string() ), \
            ( isUserChanOper(srv, *it, channel) ? std::string("@") 
                                                : std::string() ), \
            (*it)->getFullname())));
    }
    if (params.size() > 0)
        name = params[0];
    srv->sendClient(fd, numericReply(srv, fd, "315", RPL_ENDOFWHO(name)));
}
