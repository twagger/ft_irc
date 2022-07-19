/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:52:06 by twagner           #+#    #+#             */
/*   Updated: 2022/07/19 14:25:14 by twagner          ###   ########.fr       */
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

#include "../../includes/irc.hpp"
#include "../../includes/utils.hpp"

#define BACKLOG 10
#define MAX_EVENTS 10
#define BUF_SIZE 10

int server(int port, std::string password)
{
    // socket
    int                 sockfd;
    int                 nfds;
	int					new_fd;
    struct sockaddr_in  srv_addr;
    socklen_t           sin_size;
    // epoll
    int                 epollfd;
    int                 nfds;
    struct epoll_event  ev;
    struct epoll_event  events[MAX_EVENTS];
    // message
    char                buf[BUF_SIZE];
    ssize_t             ret;

    (void)password;

    // socket creation ------------------------------------------------------- /
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        return (print_error("Socket error: ", 1, true));
    
    // socket non blocking --------------------------------------------------- /
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
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

    // adding current fd to epoll interest list so we can loop on it to accept /
    // connections ----------------------------------------------------------- /
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
        return (print_error("Nezw fd add error: ", 1, true));

    // server loop ----------------------------------------------------------- /
    while (1)
    {
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) 
       		return (print_error("Epoll wait error: ", 1, true));
		for (int n = 0; n < nfds; ++n) {
			if (events[n].data.fd == sockfd) {
				sin_size = sizeof(struct sockaddr_in);
				new_fd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&srv_addr), 
							&sin_size);
				// create a fd for each connect request (accept all incoming connection)
				if (new_fd == -1) {
					print_error("Accept error: ", 1, true);
					exit(EXIT_FAILURE);
				}
				if (fcntl(new_fd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK) == -1)
					return (print_error("Socket ctl error: ", 1, true));
				ev.events = EPOLLIN;
				ev.data.fd = new_fd;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, new_fd, &ev) == -1) {
					return (print_error("Epoll ctl error: ", 1, true));
							exit(EXIT_FAILURE);
				}
			}
			else if (events[n].events & EPOLLIN) {
				//do_use_fd(events[n].data.fd);
					char buf[BUF_SIZE] = {0};
					int nbytes = read(events[n].data.fd, buf, sizeof(buf));
					// std::cout << "received bytes: " << nbytes << std::endl;
					// std::cout << buf << std::endl;
					// std::cout << "nfds" << nfds << std::endl;					
			}
		// write someting uppon reception
		std::cout << "Received connection " << std::endl;
		}
	}
    return (0);
}