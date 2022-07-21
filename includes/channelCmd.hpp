#ifndef CHANNELCMD_HPP
#define CHANNELCMD_HPP

#include <vector>
#include <iostream>
#include "user.hpp"
#include "channel.hpp"
#include "Server.hpp"

// JOIN
std::vector<std::string> getChannel(std::vector<std::string> parameter);
std::vector<std::string> getKey(std::vector<std::string> parameter);
void createChannel(std::vector<std::string> channel, std::vector<std::string> key,
    User *currentUser, Server *server);
bool channelAlreadyExists(std::map<std::string, Channel *> channelList,
    std::string channelName, User *currentUser);
void join(int fdUser, std::vector<std::string> parameter, Server *server);

#endif