/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 11:43:35 by twagner           #+#    #+#             */
/*   Updated: 2022/07/20 18:34:25 by twagner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <cstdlib>
#include <sstream>

#include "../includes/irc.hpp"
#include "../includes/utils.hpp"
#include "../includes/Server.hpp"

#define DEF_PORT 6667
#define DEF_PASS "changeme"

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

    // server creation ------------------------------------------------------- /
    Server irc_server(port, password);
    irc_server.start();
    return (0);
}
