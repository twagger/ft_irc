/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:52:06 by twagner           #+#    #+#             */
/*   Updated: 2022/07/19 11:58:26 by twagner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include "../../includes/irc.hpp"
#include "../../includes/utils.hpp"

#define BACKLOG 10

int server(int port, std::string password)
{
    // socket
    int                 sockfd;
    int                 newfd;
    struct sockaddr_in  srv_addr;
    struct sockaddr_in  cli_addr;
    socklen_t           sin_size;
    // epoll
    int                 epollfd;
    struct epoll_event  ev;

    (void)password;

    // socket creation ------------------------------------------------------- /
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        return (print_error("Socket error: ", 1, true));
    
    // socket non blocking --------------------------------------------------- /
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK) == -1)
        return (print_error("Socket ctl error: ", 1, true));

    // binding to ip address + port ------------------------------------------ /
    srv_addr.sin_family = AF_INET; // host byte order
    srv_addr.sin_port = htons(port);
    srv_addr.sin_addr.s_addr = INADDR_ANY; // autofill with any ip
    bzero(&(srv_addr.sin_zero), 8); // fill with 0
    if (bind(sockfd, reinterpret_cast<struct sockaddr*>(&srv_addr), \
        sizeof(struct sockaddr)) == -1)
        return (print_error("Bind error: ", 1, true));

    // mark the socket as passive and set a max connection backlog ----------- /
    if (listen(sockfd, BACKLOG) == -1)
        return (print_error("Listen error: ", 1, true));

    // epoll creation -------------------------------------------------------- /
    epollfd = epoll_create1(0);
    if (epollfd == -1)
        return (print_error("Epoll create error: ", 1, true));

    // adding current fd to epoll interest list ------------------------------ /
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
        return (print_error("Epoll ctl error: ", 1, true));

    // server loop ----------------------------------------------------------- /
    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        // create a fd for each connect request (accept all incoming connection)
        newfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&srv_addr), \
                     &sin_size);
        if (newfd == -1)
        {
            print_error("Accept error: ", 1, true);
            continue;
        }

        // add the fd to epoll
        ev.events = EPOLLIN;
        ev.data.fd = newfd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newfd, &ev) == -1)
            return (print_error("Epoll ctl error: ", 1, true));
        

        // write someting uppon reception
        std::cout << "Received from : " 
                  << inet_ntoa(cli_addr.sin_addr) 
                  << std::endl;
    }

    return (0);
}