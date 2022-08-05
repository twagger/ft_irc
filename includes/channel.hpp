#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <deque>
#include <string>
#include <vector>
#include <algorithm>
#include "User.hpp"

//   The available modes are as follows:
//		none ?												=> 0	0000 0000
//		o - channel operator;						        => 1	0000 0001
//     	k - key locked;                  					=> 3	0000 0010
//		i - invite only;        							=> 3	0000 0100
//      b - ban user;         								=> 4	0000 1000

#define MOD_NONE (0 << 0)
#define MOD_OPERATOR (1 << 0)
#define MOD_KEY (1 << 2)
#define MOD_INVITE (1 << 3)
#define MOD_BAN (1 << 4)

class Channel
{
private:
    std::string _topic;
    std::string _channelName;
    std::string _key;

public:
    /** Public attributes **/
    short                 _mode;
    std::deque<User *>      _operators;
    std::deque<User *>      _users;
    std::deque<std::string> _bannedUsers;
    std::deque<User *>      _invitees;

    /** Constructors and destructor **/

    Channel(std::string name, User *currentUser);
    Channel(std::string name, std::string key, User *currentUser);
    ~Channel(void);
    Channel(const Channel &cpy);

    Channel &operator=(Channel const &cpy);

    /** Getters **/

    std::string getTopic(void) const;
    std::string getChannelName(void) const;
    std::string getKey(void) const;
    std::deque<User *> getUsers(void) const;
    int getLimitNumberOfUsers(void) const;

    /** Setters **/

    void setTopic(std::string topic);
    void setKey(std::string key);
    void setMode(char mode);

    /** Member functions **/

    void addUser(User *newUser);
    void removeUser(User *userToDelete);
    void removeOperator(User *userToDelete);
    void removeBannedUser(std::string userToDelete);
    void removeInvitee(User *userToDelete);
    void addOperator(User *newOperator);
    void addBannedUser(std::string newBannedUser);
    void addInvitee(User *newInvitee);

    // mode
    void addMode(short mode);
    void removeMode(short mode);
    bool hasMode(short mode);
};

#endif
