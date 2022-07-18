/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 11:43:35 by twagner           #+#    #+#             */
/*   Updated: 2022/07/18 12:49:02 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"
#include <netinet/in.h> // struct sockaddr_in, struct in_addr
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 6667
#define BACKLOG 10
#define PASS "pass"

int main()
{
    int                 sockfd;
    int                 new_fd;
    struct sockaddr_in  srv_addr;
    struct sockaddr_in  cli_addr;
    socklen_t           sin_size;

    // socket creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cout << "Socket error: " << std::strerror(errno) << std::endl;
        return (1);
    }

    // binding to my ip address + port
    srv_addr.sin_family = AF_INET; // host byte order
    srv_addr.sin_port = htons(PORT);
    srv_addr.sin_addr.s_addr = INADDR_ANY; // autofill with my ip
    bzero(&(srv_addr.sin_zero), 8); // fill with 0
    if (bind(sockfd, reinterpret_cast<struct sockaddr*>(&srv_addr), \
        sizeof(struct sockaddr)) == -1)
    {
        std::cout << "Bind error: " << std::strerror(errno) << std::endl;
        return (1);
    }

    // go to listen mode
    if (listen(sockfd, BACKLOG) == -1)
    {
        std::cout << "Listen error: " << std::strerror(errno) << std::endl;
        return (1);
    }
    std::cout << "srv ip   : " << inet_ntoa(srv_addr.sin_addr) << std::endl
              << "srv port : " << ntohs(srv_addr.sin_port) << std::endl; 

    // accept loop
    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        new_fd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&srv_addr), \
                        &sin_size);
        if (new_fd == -1)
        {
            std::cout << "Accept error: " << std::strerror(errno) << std::endl;
            continue;
        }
        // write someting uppon reception
        std::cout << "Received from : " 
                  << inet_ntoa(cli_addr.sin_addr) 
                  << std::endl;
    }

    return (0);
}
