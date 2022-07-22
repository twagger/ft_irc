#ifndef CHANNELCMD_HPP
#define CHANNELCMD_HPP

#include <vector>
#include <iostream>
#include "user.hpp"
#include "channel.hpp"
#include "Server.hpp"

// JOIN

void join(int fdUser, std::vector<std::string> parameter, Server *server);

#endif