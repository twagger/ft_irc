/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:52:06 by twagner           #+#    #+#             */
/*   Updated: 2022/07/20 09:59:06 by codespace        ###   ########.fr       */
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
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <vector>

#include "Server.hpp"
#include "../../includes/irc.hpp"
#include "../../includes/utils.hpp"

#define BACKLOG 10
#define MAX_EVENTS 10
#define BUF_SIZE 513

// Constructors & destructor
Server::Server(Server const &src)
{ *this = src; }

// Assignment operator
Server  &Server::operator=(Server const &rhs)
{
    if (*this != rhs) {
        *this->_port = src._port;
        *this->_password = src._password;
        *this->_name = src._name;
    }
    return (*this);
}

// Getters
int         &Server::getPort(void) const 
{ return this->_port; }

std::string &Server::getPassword(void) const
{ return this->_password; }

std::string &Server::getName(void) const
{ return this->_name; }

// Member functions
Server::start(void)
{
    // socket
    int                                         sockfd;
	int					                        newfd;
    struct sockaddr_in                          srv_addr;
    socklen_t                                   sin_size;
    // epoll
    int                                         epollfd;
    int                                         nfds;
    struct epoll_event                          ev;
    struct epoll_event                          events[MAX_EVENTS];
    // message / command  
    char                                        buf[BUF_SIZE];
    ssize_t                                     ret;
    std::string                                 mess;
    std::map< int, std::vector<std::string> >   cmd;

    // socket creation ------------------------------------------------------- /
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        return (print_error("Socket error: ", 1, true));
    
    // socket non blocking --------------------------------------------------- /
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
        return (print_error("Socket ctl error: ", 1, true));

    // binding to ip address + port ------------------------------------------ /
    srv_addr.sin_family = AF_INET; // host byte order
    srv_addr.sin_port = htons(this._port);
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

    // adding current fd to epoll interest list so we can loop on it to accept /
    // connections ----------------------------------------------------------- /
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
        return (print_error("New fd add error: ", 1, true));

    // server loop ----------------------------------------------------------- /
    while (1)
    {
        // epoll wait -------------------------------------------------------- /
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1)
            return (print_error("Epoll wait error: ", 1, true));

        // loop on ready fds ------------------------------------------------- /
        for (int i = 0; i < nfds; ++i)
        {
            // check if sockfd is receiving new connection request ----------- /
            if (events[i].data.fd == sockfd)            
            {
                // accept the connect request
                newfd = accept(sockfd, \
                        reinterpret_cast<struct sockaddr*>(&srv_addr), \
                        &sin_size);
                if (newfd == -1)
                {
                    print_error("Accept error: ", 1, true);
                    continue;
                }
                // add the new fd to the epoll
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = newfd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newfd, &ev) == -1)
                    return (print_error("New fd add error: ", 1, true));
            }
            else // message from existing connection ------------------------- /
            {
                memset(buf, 0, BUF_SIZE);
                ret = recv(events[i].data.fd, buf, BUF_SIZE, 0);
                buf[ret] = '\0';
                std::cout << "Message received from (" << events[i].data.fd 
                          << ") : " << buf << std::endl;
                
                // split the message and create a map with the fd as a key
                mess = static_cast<std::string>(buf);
                while (mess.length() > 0) 
                    cmd[events[i].data.fd].push_back(get_next_tokn(&mess, " "));

                // parse the command from the map ---------------------------- /
                if (this._cmd_list[CMD].exec_command(FD, CMD, PARAM) == -1)
                    // send an error to client

            }
        }
    }

    return (0);
}
