#include "../../includes/utils.hpp"
#include <string>
#include <cstring>
#include <iostream>
#include <cerrno>

int printError(std::string message, int code, bool with_errno = false)
{
    std::cout << message;
    if (with_errno)
        std::cout << std::strerror(errno);
    std::cout << std::endl;
    return (code);
}
