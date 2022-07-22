#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <deque>
#include <string>
#include <vector>
#include <algorithm>
#include "user.hpp"

class Channel
{
    private:
    
    std::string             _topic;
    std::string             _channelName;
    std::string             _key;
    std::vector<char>       _mode;   

    public:

/** Public attributes **/
    std::deque<User*>       _operators;
    std::deque<User*>       _users;
    std::deque<User*>       _bannedUsers;
    std::deque<User*>       _invitees;

/** Constructors and destructor **/

    Channel(std::string name, User *currentUser);
    Channel(std::string name, std::string key, User *currentUser);
    ~Channel(void);

/** Getters **/

    std::string getTopic(void) const;
    std::string getChannelName(void) const;
    std::string getKey(void) const;
    int getLimitNumberOfUsers(void) const;

/** Setters **/
    
    void setTopic(std::string topic);
    void setKey(std::string key);
    void setMode(char mode);

/** Member functions **/

    void removeMode(char mode);
    void addUser(User *newUser);
    void removeUser(User *userToDelete);
    void addOperator(User *newOperator);
    void addBannedUser(User *newBannedUser);
    void addInvitee(User *newInvitee);
};

#endif