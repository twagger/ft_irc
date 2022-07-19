/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 11:43:35 by twagner           #+#    #+#             */
/*   Updated: 2022/07/19 17:56:22 by twagner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <cstdlib>
#include <sstream>

#include "../includes/irc.hpp"
#include "../includes/utils.hpp"

#define DEF_PORT 6667
#define DEF_PASS "changeme"

int main(int ac, char **av)
{
    // parameters
    stringstream    ss; 
    int             port = DEF_PORT;
    std::string     password = DEF_PASS;
    char            *end;

    // basic using parameters ------------------------------------------------ /
    // (add more control later)
    if (ac != 3)
        return (print_error("Parameters error", 1, false));
    ss << av[1];
    ss >> port;
    password = av[2]; 

    // server call ----------------------------------------------------------- /
    return (server(port, password));
}
