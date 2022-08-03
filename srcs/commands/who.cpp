#include <algorithm>

#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"
#include "../../includes/parser.hpp"

bool    isOnTheSameChannel(Server *srv, const int &fd1, const int &fd2)
{
    std::deque<std::string>::const_iterator it;
    std::deque<std::string>::const_iterator itRes;
    User                                    *user1 = srv->getUserByFd(fd1);
    User                                    *user2 = srv->getUserByFd(fd2);

    for (it = user1->getChannelsJoined().begin(); \
         it != user1->getChannelsJoined().end(); ++it)
    {
        itRes = std::find(user2->getChannelsJoined().begin(), \
                  user2->getChannelsJoined().end(), *it);
        if (itRes != user2->getChannelsJoined().end())
            return (true);
    }
    return (false);
}

// Send information about users visible for me
void who(const int &fd, const std::vector<std::string> &params, \
         const std::string &, Server *srv)
{
    std::string                 mask;
    bool                        onlyOpers = false;
    std::deque<User*>           usersList;
    std::deque<User*>::iterator it;

    // COMMAND EXECUTION
    // Params
    if (params.size() >= 1)
    {
        mask = params[0];
        if (params.size() > 1 and params[1].compare("o") == 0)
            onlyOpers = true;
    }

    // Try to match all users visible to me : same channel or non "i"
    for (it = srv->getAllUsers().begin(); it != srv->getAllUsers().end(); it++)
    {
        // Check if user is non invisible
        if ((*it)->hasMode(MOD_INVISIBLE) == false) 
            usersList.push_back(*it);
        else if (isOnTheSameChannel(srv, fd, (*it)->getFd()) == true)
            usersList.push_back(*it);
    }

    if (srv->_channelList.find(mask) != srv->_channelList.end()) {
        // The mask match a channel
        for (it = usersList.begin(); it != usersList.end(); it++)
        {
            if (std::find((*it)->getChannelsJoined().begin(), \
                (*it)->getChannelsJoined().end(), mask) \
                    == (*it)->getChannelsJoined().end())
            {
                usersList.erase(it);
            }
        }
    }
    else {
        // Check the mask against users's Host, Server, Real name, Nickname
        for (it = usersList.begin(); it != usersList.end(); it++)
        {
            // Host
            if (matchMask((*it)->getHostname().c_str(), mask.c_str()) == true)
                continue;
            // Server
            if (matchMask(srv->getHostname().c_str(), mask.c_str()) == true)
                continue;
            // Real name
            if (matchMask((*it)->getFullname().c_str(), mask.c_str()) == true)
                continue;
            // Nickname
            if (matchMask((*it)->getNickname().c_str(), mask.c_str()) == true)
                continue;
            usersList.erase(it);
        }
    }

    if (onlyOpers == true) {
        // Apply oper option on the list
        for (it = usersList.begin(); it != usersList.end(); it++)
        {
            // If user hasMode(MOD_OPER) == false > Remove
            if ((*it)->hasMode(MOD_OPER) == false)
                usersList.erase(it);
        }
    }

    // Send information about all users in the list
    
}
