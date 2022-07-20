/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erecuero <erecuero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:52:06 by twagner           #+#    #+#             */
/*   Updated: 2022/07/20 17:23:55 by erecuero         ###   ########.fr       */
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
#define BUF_SIZE 4096
#define MAX_EVENTS 10

/* ************************************************************************** */
/* Constructors & destructor                                                  */
/* ************************************************************************** */
Server::Server(int port, std::string password, std::string name)
: _port(port), _password(password), _name(name) {}

Server::Server(Server const &src)
{ *this = src; }

/* ************************************************************************** */
/* Operator overloads                                                         */
/* ************************************************************************** */
Server  &Server::operator=(Server const &rhs)
{
    if (this != &rhs) {
        this->_port = rhs._port;
        this->_password = rhs._password;
        this->_name = rhs._name;
    }
    return (*this);
}

/* ************************************************************************** */
/* Getters                                                                    */
/* ************************************************************************** */
int Server::getPort(void) const 
{ return this->_port; }

std::string Server::getPassword(void) const
{ return this->_password; }

std::string Server::getName(void) const
{return this->_name; }

User*		Server::getUserByFd(const int &fd) const
{
	std::map<int, User *>::const_iterator ite = this->_user_list.end();

	if (this->_user_list.find(fd) != ite)
		return (this->_user_list.find(fd)->second);
	return (NULL);
}

/* ************************************************************************** */
/* Private member functions                                                   */
/* ************************************************************************** */
int Server::_create_socket(void)
{
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        throw Server::SocketException();
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
        throw Server::SocketException();
    return (sockfd);
}

void    Server::_bind_socket(int sockfd, struct sockaddr_in *srv_addr)
{
    // binding to ip address + port
    srv_addr->sin_family = AF_INET; // host byte order
    srv_addr->sin_port = htons(this->_port);
    srv_addr->sin_addr.s_addr = INADDR_ANY; // autofill with any ip
    bzero(&(srv_addr->sin_zero), 8); // fill with 0
    if (bind(sockfd, reinterpret_cast<struct sockaddr *>(srv_addr), \
        sizeof(struct sockaddr)) == -1)
        throw Server::BindException();

    // mark the socket as passive and set a max connection backlog
    if (listen(sockfd, BACKLOG) == -1)
        throw Server::BindException();
}

int Server::_create_poll(int sockfd)
{
    int                 pollfd;
    struct epoll_event  ev;

    // epoll creation
    pollfd = epoll_create1(0);
    if (pollfd == -1)
        throw Server::PollException();

    // adding current fd to epoll interest list so we can loop on it to accept
    // connections
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sockfd;
    if (epoll_ctl(pollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
        throw Server::PollException();
    return (pollfd);
}

int Server::_poll_wait(int pollfd, struct epoll_event **events, int max_events)
{
    int nfds;

    nfds = epoll_wait(pollfd, *events, max_events, -1);
    if (nfds == -1)
        throw Server::PollWaitException();
    return (nfds);
}

void    Server::_accept_connection(\
                int sockfd, int pollfd, struct sockaddr_in *srv_addr)
{
    socklen_t           sin_size;
    struct epoll_event  ev;
	int					newfd;

    // accept the connect request
    newfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(srv_addr), \
        &sin_size);
    if (newfd == -1)
        throw Server::AcceptException();

    // add the new fd to the poll
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = newfd;
    if (epoll_ctl(pollfd, EPOLL_CTL_ADD, newfd, &ev) == -1)
        throw Server::AcceptException();
}

void    Server::_handle_new_message(struct epoll_event event)
{
    // message / command  
    char                                        buf[BUF_SIZE];
    ssize_t                                     ret;
    std::string                                 mess;
    std::map< int, std::vector<std::string> >   cmd;

    memset(buf, 0, BUF_SIZE);
    ret = recv(event.data.fd, buf, BUF_SIZE, 0);
    buf[ret] = '\0';
    std::cout << "Message received from (" << event.data.fd 
                << ") : " << buf << std::endl;
    
    // split the message and create a map with the fd as a key
    mess = static_cast<std::string>(buf);
    while (mess.length() > 0) 
        cmd[event.data.fd].push_back(get_next_tokn(&mess, " "));

    // parse the command from the map ---------------------------- /
    //if (this._cmd_list[CMD].exec_command(FD, CMD, PARAM) == -1)
        // send an error to client
}

/* ************************************************************************** */
/* Member functions                                                           */
/* ************************************************************************** */
void    Server::start(void)
{
    // socket
    int                 sockfd;
    struct sockaddr_in  srv_addr;
    // epoll
    int                 pollfd;
    int                 nfds;
    struct epoll_event  events[MAX_EVENTS];
    struct epoll_event  *events_tmp;

    // socket creation and param --------------------------------------------- /
    try { sockfd = this->_create_socket(); }
    catch (Server::SocketException &e){ print_error(e.what(), 1, true); return;}

    // binding to ip address + port and switch to passive mode --------------- /
    try { this->_bind_socket(sockfd, &srv_addr); }
    catch (Server::BindException &e) { print_error(e.what(), 1, true); return; }

    // poll creation --------------------------------------------------------- /
    try { pollfd = this->_create_poll(sockfd); }
    catch (Server::PollException &e) { print_error(e.what(), 1, true); return; }

    // server loop ----------------------------------------------------------- /
    while (1)
    {
        // poll wait --------------------------------------------------------- /
        events_tmp = &(events[0]);
        try { nfds = this->_poll_wait(pollfd, &events_tmp, MAX_EVENTS); }
        catch (Server::PollWaitException &e)
        { print_error(e.what(), 1, true); return; }

        // loop on ready fds ------------------------------------------------- /
        for (int i = 0; i < nfds; ++i)
        {
            // handle new connection requests -------------------------------- /
            if (events[i].data.fd == sockfd)            
            {
                try { this->_accept_connection(sockfd, pollfd, &srv_addr); }
                catch (Server::AcceptException &e)
                { print_error(e.what(), 1, true); return; }
                catch (Server::PollAddException &e) {}
            }
            else // new message from existing connection --------------------- /
            {
                try { this->_handle_new_message(events[i]); }
                catch (std::exception &e) { }
            }
        }
    }
}

/* ************************************************************************** */
/* Exceptions                                                                 */
/* ************************************************************************** */
const char	*Server::SocketException::what() const throw()
{ return ("Socket creation or mode error: "); }

const char	*Server::BindException::what() const throw()
{ return ("Bind error: "); }

const char	*Server::PollException::what() const throw()
{ return ("Poll error: "); }

const char	*Server::PollWaitException::what() const throw()
{ return ("Poll wail error: "); }

const char	*Server::PollAddException::what() const throw()
{ return ("Poll add error: "); }

const char	*Server::AcceptException::what() const throw()
{ return ("Accept error: "); }
