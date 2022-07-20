#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "user.hpp"

class Channel
{
    private:
    
    std::string             _topic;
    std::string             _channelName;
    std::string             _key;
    std::map<int, User*>    _operators;
    std::map<int, User*>    _users;
    std::map<int, User*>    _bannedUsers;
    std::map<int, User*>    _invitee;
    std::vector<char>       _mode;
    int                     _limitNumberOfUsers;
   

    public:

    Channel(std::string name);
    Channel(std::string name, std::string key);
    ~Channel(void);

    std::string getTopic(void) const;
    std::string getChannelName(void) const;
    std::string getKey(void) const;
};

#endif