#include <cstring>
#include <cstdlib>
#include <sstream>


#include "../includes/irc.hpp"
#include "../includes/utils.hpp"
#include "../includes/Server.hpp"

#define DEF_PORT 6667
#define DEF_PASS "pwd"

#include <iostream>
#include <csignal>
# include <stdlib.h>
# include <stdio.h>

void signalHandler( int signum ) {
	(void)signum;
	std::cout << "\r  ";
}


int main(int ac, char **av)
{
    // parameters
    std::stringstream   ss; 
	int                 port = DEF_PORT;
	std::string         password = DEF_PASS;
	
    // basic using parameters ------------------------------------------------ /
    if (ac != 3)
        return (printError("Parameters error", 1, false));
    ss << av[1];
    ss >> port;
    password = av[2];

	// signal CTRL-C handler ------------------------------------------------- /
	signal(SIGINT, signalHandler);

    // server creation ------------------------------------------------------- /
    Server ircServer(port, password);
    ircServer.start();
    return (0);
}
