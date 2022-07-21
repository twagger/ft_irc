#include "../../includes/utils.hpp"

// Split a string by \n\r and return a string vector
std::vector<std::string>    splitBy(std::string str, std::string delimiter)
{
    std::vector<std::string>    result;
    int                         end;

    // first check 
    end = str.find(delimiter);
    if (end == -1)
        throw std::runtime_error("IRC message must end with CRLF");
    // save first command in vector
    if (end + 2 > MAX_CMD_LEN)
        throw std::runtime_error("IRC message shall not exceed 512 characters");
    result.push_back(str.substr(0, end));
    // update str
    str.erase(0, end + delimiter.length());
    // loop for other commands
    end = str.find(delimiter);
    while (end != -1)
    {
        if (end + 2 > MAX_CMD_LEN)
            throw std::runtime_error(\
                                 "IRC message shall not exceed 512 characters");
        result.push_back(str.substr(0, end));
        str.erase(0, end + delimiter.length());
        end = str.find(delimiter);
    }
    return (result);
}
