#include "../../includes/channel.hpp"
#include "../../includes/returnCode.hpp"
#include "../../includes/utils.hpp"

std::vector<std::string> getChannel(std::vector<std::string> parameter)
{
    std::vector<std::string>::iterator it = parameter.begin();
    std::vector<std::string> channel;

    // Case if parameter given is a 0
    std::string tmp = *it;
    if (tmp.compare("0") == 0)
    {
        channel.push_back("0");
        // part();
        return (channel);
    }
    channel = splitByComma(tmp);
    for (it = channel.begin(); it != channel.end(); it++)
    {
        if (isChannel(*it) == false)
        {
            channel.erase(it);
        }
    }
    return (channel);
}

std::vector<std::string> getKey(std::vector<std::string> parameter)
{
    std::vector<std::string>::iterator it = parameter.end();

    return(split_by_comma(*it));
}

void createChannel(std::vector<std::string> channel, std::vector<std::string> key)
{
    std::vector<std::string>::iterator it = channel.begin();
    std::vector<std::string>::iterator it2 = key.begin();

    for (; it != channel.end(); it++)
    {
        if (key.empty() == false || it2 != key.end())
        {
            Channel newChannel(*it, *it2);
            it2++;
        }
        else
        {
            Channel newChannel(*it);
        }
        // Function to add new channel to list in server
        // addChanneltoList(newChannel);
    }
}

void join(std::vector<std::string> parameter)
{
    // Check parameters of join

    std::vector<std::string> channel;
    std::vector<std::string> key;

    channel = getChannel(parameter);
    std::vector<std::string>::iterator it = channel.begin();
    std::string tmp = *it;
    if (tmp.compare("0") == 0)
    {
        return ;
    }
    if (channel.empty() == true)
    {
        std::cout << ERR_NEEDMOREPARAMS << std::endl;
        std::cout << "JOIN :Not enough parameters" << std::endl;
        return ;
    }
    if (parameter.size() > 1)
    {
        key = getKey(parameter);
    }

    // Check if channel already exists
    // Make a function that allows to look for a string inside the private argument channel_list
    // Create channel
    createChannel(channel, key);
}