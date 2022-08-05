#include "../../includes/channel.hpp"

Channel::Channel(std::string name, User *currentUser): _channelName(name),
    _mode(MOD_NONE)
            {
                addOperator(currentUser);
                addUser(currentUser);
            }

Channel::Channel(std::string name, std::string key, User *currentUser): _channelName(name),
    _key(key), _mode(MOD_NONE)
            {
                addOperator(currentUser);
                addUser(currentUser);
                this->addMode(MOD_KEY);
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

void Channel::addUser(User *newUser)
{
    this->_users.push_back(newUser);
}

void Channel::addOperator(User *newOperator)
{
    this->_operators.push_back(newOperator);
}

void Channel::addBannedUser(std::string newBannedUser)
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

void Channel::removeOperator(User *userToDelete)
{
    std::deque<User *>::iterator pos = std::find(this->_operators.begin(),
        this->_operators.end(), userToDelete);
    if (pos != this->_operators.end())
    {
        this->_operators.erase(pos);
    }
}

void Channel::removeBannedUser(std::string userToDelete)
{
    std::deque<std::string>::iterator pos = std::find(this->_bannedUsers.begin(),
        this->_bannedUsers.end(), userToDelete);
    if (pos != this->_bannedUsers.end())
    {
        this->_bannedUsers.erase(pos);
    }
}

void Channel::removeInvitee(User *userToDelete)
{
    std::deque<User *>::iterator pos = std::find(this->_invitees.begin(),
        this->_invitees.end(), userToDelete);
    if (pos != this->_invitees.end())
    {
        this->_invitees.erase(pos);
    }
}

/* Mode */

void Channel::addMode(short mode) { this->_mode |= mode; }
void Channel::removeMode(short mode) { this->_mode &= ~mode; }
bool Channel::hasMode(short mode) { return ((this->_mode & mode) > 0); }
