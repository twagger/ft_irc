#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "user.hpp"

class Channel
{
    private:
    
    std::string _topic;
    std::string _name;
    std::string _key;
    std::map<int, User*> _operators;
    std::map<int, User*> _users;
    std::map<int, User*> _invitee;
   

    public:

    Channel(std::string name, std::string key): _topic(0), _name(name), _key(key),
            _operators(0), _users(0), _invitee(0) {};
    ~Channel(void) {};

    std::string getTopic(void) const { return (this->_topic); };
    std::string getName(void) const { return (this->_name); };
    std::string getKey(void) const { return (this->_key); };
};

#endif COMMAND_HPP