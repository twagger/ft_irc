#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

std::string pong(int fd, std::vector<std::string> params, Server *srv)
{
    std::string clientname;

    (void)fd;
    (void)srv;
    // check nb of param
    if (params.size() == 1) // only clientname is expected
    {
        clientname = params[0];
        std::cout << "client : "  << clientname << std::endl;
    }
    return (NULL);
}
