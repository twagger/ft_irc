#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <map>
#include <vector>

class Channel
{
    private:

    std::map<int, ConnectedClient *> _operators;
    std::map<int, ConnectedClient*> _users;
};

#endif COMMAND_HPP