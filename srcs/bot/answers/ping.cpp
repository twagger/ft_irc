#include <string>
#include <deque>
#include <cstdlib>
#include <fstream>

std::string botPing(const std::string &message)
{
    std::string response;

    response
        .append("PONG ")
        .append(message.substr(message.find("PING") + 5));

    return (response);
}
