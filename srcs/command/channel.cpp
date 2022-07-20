#include "../../includes/channel.hpp"

Channel::Channel(std::string name): _topic(0), _channelName(name), _key(0),
            _operators(0), _users(0), _bannedUsers(0), _invitee(0), _mode(0), _limitNumberOfUsers(-1) {};

Channel::Channel(std::string name, std::string key): _topic(0), _channelName(name), _key(key),
            _operators(0), _users(0), _bannedUsers(0), _invitee(0), _limitNumberOfUsers(-1)
            {
                this->_mode.push_back('k');
            };

Channel::~Channel(void) {};

std::string Channel::getTopic(void) const { return (this->_topic); };

std::string Channel::getChannelName(void) const { return (this->_channelName); };

std::string Channel::getKey(void) const { return (this->_key); };