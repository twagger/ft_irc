#include "../../includes/channel.hpp"

Channel::Channel(std::string name): _topic(0), _channelName(name), _key(0), _mode(0),
            _operators(0), _users(0), _bannedUsers(0), _invitees(0), _limitNumberOfUsers(-1) {}

Channel::Channel(std::string name, std::string key): _topic(0), _channelName(name), _key(key),
            _operators(0), _users(0), _bannedUsers(0), _invitees(0), _limitNumberOfUsers(-1)
            {
                this->_mode.push_back('k');
            }

Channel::~Channel(void) {}

/** Getters **/

std::string Channel::getTopic(void) const { return (this->_topic); }

std::string Channel::getChannelName(void) const { return (this->_channelName); }

std::string Channel::getKey(void) const { return (this->_key); }

int Channel::getLimitNumberOfUsers(void) const { return (this->_limitNumberOfUsers); }


/** Setters **/

void Channel::setTopic(std::string topic) { this->_topic = topic; }

void Channel::setKey(std::string key) { this->_key = key; }

void Channel::setMode(char mode) { this->_mode.push_back(mode); }

void Channel::setLimitNumberofUsers(int limitNumberOfUsers) { this->_limitNumberOfUsers = limitNumberOfUsers; }


/**
 * @brief Remove a mode in the channel
 * 
 * @param mode passed as argument with -
 */

void Channel::removeMode(char mode)
{
    std::vector<char>::iterator pos = std::find(this->_mode.begin(), this->_mode.end(), mode);
    if (pos != this->_mode.end())
    {
        this->_mode.erase(pos);
    }
}

void Channel::addUser(User *newUser)
{
    this->_users.push_back(newUser);
}

void Channel::addOperator(User *newOperator)
{
    this->_operators.push_back(newOperator);
}

void Channel::addBannedUser(User *newBannedUser)
{
    this->_bannedUsers.push_back(newBannedUser);
}

void Channel::addInvitee(User *newInvitee)
{
    this->_invitees.push_back(newInvitee);
}

void Channel::removeUser(User *userToDelete)
{
    std::deque<User *>::iterator pos = std::find(this->_users.begin(), this->_users.end(), userToDelete);
    if (pos != this->_users.end())
    {
        this->_users.erase(pos);
    }
}