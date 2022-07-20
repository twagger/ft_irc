#include <sstream>

#include "../../includes/utils.hpp"
#include "../../includes/Server.hpp"

bool is_channel(std::string channelName)
{
    if (channelName.find("#") != std::string::npos
        || channelName.find("&") != std::string::npos)
        {
            return (true);
        }
    return (false);
}

std::vector<std::string> split_by_comma(std::string parameter)
{
    std::vector<std::string> tab;
    std::string temp;
    std::istringstream stream(parameter);
    while (std::getline(stream, temp, ','))
    {
        tab.push_back(temp);
    }
    return (tab);
}

/**
 * @brief Convert a vector of strings into a vector of Command by splitting 
 *        parameters and command
 * 
 * @param cmd_strings Vector of commands, not splitted by space
 * @return vector of t_command where CMD and PARAMS are separated
 */
std::vector<Command>  splitCmds(std::vector<std::string> cmd_strings)
{
    std::vector<Command>                result;
    std::vector<std::string>::iterator  it;
    int                                 end;

    for (it = cmd_strings.begin(); it != cmd_strings.end(); ++it)
    {
        // extract CMD name 
        end = it->find(' ');
        if (end == -1) // only command
            result.push_back(*(new Command(it->substr(0, end))));
        else // command + params
        {
            // CMD
            result.push_back(*(new Command(it->substr(0, end))));
            // PARAMS
            it->erase(0, end + 1);
            end = it->find(' ');
            while (end != -1)
            {
                result.back().params.push_back(it->substr(0, end));
                it->erase(0, end + 1);
                end = it->find(' ');
            }
        }
    }
    
    return (result);
}
