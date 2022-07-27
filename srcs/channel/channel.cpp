#include "../../includes/channel.hpp"

Channel::Channel(std::string name, User *currentUser): _channelName(name)
            {
                addUser(currentUser);
            }

Channel::Channel(std::string name, std::string key, User *currentUser): _channelName(name), _key(key)
            {
                addOperator(currentUser);
                this->_mode.push_back('k');
            }

Channel::~Channel(void) {}

Channel::Channel(const Channel &src): _topic(src._topic), _channelName(src._channelName),
    _key(src._key), _mode(src._mode), _operators(src._operators), _bannedUsers(src._bannedUsers),
    _invitees(src._invitees) { }

Channel &Channel::operator=(Channel const &cpy)
{
    if (this != &cpy)
    {
        this->_topic = cpy._topic;
        this->_channelName = cpy._channelName;
        this->_key = cpy._key;
        this->_mode = cpy._mode;
        this->_operators = cpy._operators;
        this->_bannedUsers = cpy._bannedUsers;
        this->_invitees = cpy._invitees;
    }
    return (*this);
}

/** Getters **/

std::string Channel::getTopic(void) const { return (this->_topic); }

std::string Channel::getChannelName(void) const { return (this->_channelName); }

std::string Channel::getKey(void) const { return (this->_key); }

std::deque<User*>   Channel::getUsers(void) const { return (this->_users); }

/** Setters **/

void Channel::setTopic(std::string topic) { this->_topic = topic; }

void Channel::setKey(std::string key) { this->_key = key; }

void Channel::setMode(char mode) { this->_mode.push_back(mode); }

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

void Channel::addMode(char mode)
{
    this->_mode.push_back(mode);
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
