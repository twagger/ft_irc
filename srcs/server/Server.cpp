/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:52:06 by twagner           #+#    #+#             */
/*   Updated: 2022/07/21 09:47:25 by twagner          ###   ########.fr       */
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

#include "../../includes/Server.hpp"
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

Command::Command(\
std::string cmd, std::string prefix, std::vector<std::string> params)
: command(cmd), prefix(prefix), params(params){}

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
{ return this->_name; }

/* ************************************************************************** */
/* Private member functions                                                   */
/* ************************************************************************** */
int Server::_createSocket(void)
{
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        throw Server::socketException();
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
        throw Server::socketException();
    return (sockfd);
}

void    Server::_bindSocket(int sockfd, struct sockaddr_in *srv_addr)
{
    // binding to ip address + port
    srv_addr->sin_family = AF_INET; // host byte order
    srv_addr->sin_port = htons(this->_port);
    srv_addr->sin_addr.s_addr = INADDR_ANY; // autofill with any ip
    bzero(&(srv_addr->sin_zero), 8); // fill with 0
    if (bind(sockfd, reinterpret_cast<struct sockaddr *>(srv_addr), \
        sizeof(struct sockaddr)) == -1)
        throw Server::bindException();

    // mark the socket as passive and set a max connection backlog
    if (listen(sockfd, BACKLOG) == -1)
        throw Server::bindException();
}

int Server::_createPoll(int sockfd)
{
    int                 pollfd;
    struct epoll_event  ev;

    // epoll creation
    pollfd = epoll_create1(0);
    if (pollfd == -1)
        throw Server::pollException();

    // adding current fd to epoll interest list so we can loop on it to accept
    // connections
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sockfd;
    if (epoll_ctl(pollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
        throw Server::pollException();
    return (pollfd);
}

int Server::_pollWait(int pollfd, struct epoll_event **events, int max_events)
{
    int nfds;

    nfds = epoll_wait(pollfd, *events, max_events, -1);
    if (nfds == -1)
        throw Server::pollWaitException();
    return (nfds);
}

void    Server::_acceptConnection(\
                int sockfd, int pollfd, struct sockaddr_in *srv_addr)
{
    socklen_t           sin_size;
    struct epoll_event  ev;
	int					newfd;

    // accept the connect request
    newfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(srv_addr), \
        &sin_size);
    if (newfd == -1)
        throw Server::acceptException();
    
    // add the new fd to the poll
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = newfd;
    if (epoll_ctl(pollfd, EPOLL_CTL_ADD, newfd, &ev) == -1)
        throw Server::acceptException();
}

void    Server::_handleNewMessage(struct epoll_event event)
{
    char                        buf[BUF_SIZE];
    ssize_t                     ret;
    std::string                 mess;
    std::vector< std::string >  cmd_strings;
    std::vector<Command>        cmd;

    // fill a buffer with the message
    memset(buf, 0, BUF_SIZE);
    ret = recv(event.data.fd, buf, BUF_SIZE, 0);
    buf[ret] = '\0';

    // split the commands in a vector. Non blocking in case of not ok message.
    try { cmd_strings = splitBy(buf, "\r\n"); } 
    catch (std::runtime_error &e) { printError(e.what(), 1, false); }

    // split all commands in a vector of t_command (CMD / PARAM)
    try { cmd = splitCmds(cmd_strings); }
    catch (std::runtime_error &e) {printError(e.what(), 1, false); }

    // temporary check
    std::vector<Command>::iterator  it;
    std::vector<std::string>::iterator  it2;
    for (it = cmd.begin(); it < cmd.end(); ++it)
    {
        std::cout << "\nPREFIX : " << it->prefix << std::endl;
        std::cout << "CMD : " << it->command << std::endl;
        for (it2 = it->params.begin(); it2 < it->params.end(); ++it2)
        {
            std::cout << "PARAM : " << *it2 << std::endl;
        }
    }
    // Loop on the commands and execute them (use vector iterator)
    
    // Search the proper function in cmd map and execute it with params
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
    try { sockfd = this->_createSocket(); }
    catch (Server::socketException &e){ printError(e.what(), 1, true); return;}

    // binding to ip address + port and switch to passive mode --------------- /
    try { this->_bindSocket(sockfd, &srv_addr); }
    catch (Server::bindException &e) { printError(e.what(), 1, true); return; }

    // poll creation --------------------------------------------------------- /
    try { pollfd = this->_createPoll(sockfd); }
    catch (Server::pollException &e) { printError(e.what(), 1, true); return; }

    // server loop ----------------------------------------------------------- /
    while (1)
    {
        // poll wait --------------------------------------------------------- /
        events_tmp = &(events[0]);
        try { nfds = this->_pollWait(pollfd, &events_tmp, MAX_EVENTS); }
        catch (Server::pollWaitException &e)
        { printError(e.what(), 1, true); return; }

        // loop on ready fds ------------------------------------------------- /
        for (int i = 0; i < nfds; ++i)
        {
            // handle new connection requests -------------------------------- /
            if (events[i].data.fd == sockfd)            
            {
                try { this->_acceptConnection(sockfd, pollfd, &srv_addr); }
                catch (Server::acceptException &e)
                { printError(e.what(), 1, true); return; }
                catch (Server::pollAddException &e)
                { printError(e.what(), 1, true); return; }
                catch (Server::passwordException &e)
                { printError(e.what(), 1, true); }

            }
            else // new message from existing connection --------------------- /
            {
                try { this->_handleNewMessage(events[i]); }
                catch (std::exception &e) { }
            }
        }
    }
}

/* ************************************************************************** */
/* Exceptions                                                                 */
/* ************************************************************************** */
const char	*Server::socketException::what() const throw()
{ return ("Socket creation or mode error: "); }

const char	*Server::bindException::what() const throw()
{ return ("Bind error: "); }

const char	*Server::pollException::what() const throw()
{ return ("Poll error: "); }

const char	*Server::pollWaitException::what() const throw()
{ return ("Poll wail error: "); }

const char	*Server::pollAddException::what() const throw()
{ return ("Poll add error: "); }

const char	*Server::acceptException::what() const throw()
{ return ("Accept error: "); }

const char	*Server::passwordException::what() const throw()
{ return ("Password error: please provide a correct password"); }
