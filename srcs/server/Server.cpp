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
: _port(port), _password(password), _name(name)
{ this->_initCommandList(); }

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
        this->_userList = rhs._userList;
        this->_channelList = rhs._channelList;
        this->_cmdList = rhs._cmdList;
		this->_hostname = rhs._hostname;
		this->_version = rhs._version;
		this->_date = rhs._date;
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

std::string Server::getHostname(void) const
{ return this->_hostname; }

std::string Server::getVersion(void) const
{ return this->_version; }

std::string Server::getDate(void) const
{ return this->_date; }

User*		Server::getUserByFd(const int &fd) const
{
	std::map<int, User *>::const_iterator ite = this->_userList.end();

	if (this->_userList.find(fd) != ite)
		return (this->_userList.find(fd)->second);
	return (NULL);
}

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
    srv_addr->sin_addr.s_addr = INADDR_ANY;
    memset(&(srv_addr->sin_zero), 0, 8); // fill remaining space with 0
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

void    Server::_acceptConnection(int sockfd, int pollfd)
{
    socklen_t           sin_size;
    struct epoll_event  ev;
	int					newfd;
    struct sockaddr_in  client_addr;

    // accept the connect request
    newfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&client_addr), \
        &sin_size);
    if (newfd == -1)
        throw Server::acceptException();

    // create a new empty user 
    this->_userList[newfd] = new User(newfd, inet_ntoa(client_addr.sin_addr));
    std::cout << inet_ntoa(client_addr.sin_addr) << std::endl;

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
    std::vector<Command>        cmds;

    // fill a buffer with the message
    memset(buf, 0, BUF_SIZE);
    ret = recv(event.data.fd, buf, BUF_SIZE, 0);
    if (ret == -1)
        throw std::exception();
    buf[ret] = '\0';

    // split the commands in a vector. Non blocking in case of not ok message.
    try { cmd_strings = splitBy(buf, "\r\n"); } 
    catch (std::runtime_error &e) { printError(e.what(), 1, false); }

    // split all commands in a vector of t_command (CMD / PARAM)
    try { cmds = splitCmds(cmd_strings); }
    catch (std::runtime_error &e) {printError(e.what(), 1, false); }

    // temporary check
    std::vector<Command>::iterator  it;
    std::vector<std::string>::iterator  it2;
    for (it = cmds.begin(); it < cmds.end(); ++it)
    {
        std::cout << "\nPREFIX : " << it->prefix << std::endl;
        std::cout << "CMD : " << it->command << std::endl;
        for (it2 = it->params.begin(); it2 < it->params.end(); ++it2)
        {
            std::cout << "PARAM : " << *it2 << std::endl;
        }
    }

    // execute all commands in the vector
    this->_executeCommands(event.data.fd, cmds);
}

void    Server::_initCommandList(void) // functions to complete
{
    this->_cmdList["PASS"] = &pass;
    this->_cmdList["-NICK"] = NULL;
    this->_cmdList["-USER"] = NULL;
}

void    Server::_executeCommands(const int fd, std::vector<Command> cmds)
{
    std::vector<Command>::iterator                  it;
    std::map<std::string, CmdFunction>::iterator    it_cmd;
    std::string                                     result;
    CmdFunction                                     exec_command;
    std::string                                     reply_str;
    int                                             ret;

    // for each command in the message
    for (it = cmds.begin(); it < cmds.end(); ++it)
    {
        // search if it is in the known commands list of the server
        it_cmd = this->_cmdList.find(it->command);
        if (it_cmd != this->_cmdList.end())
        {
            // execute the command
            exec_command = it_cmd->second;
            result = exec_command(fd, it->params, this);
            // send the result to the client if it is not empty
            if (!result.empty())
            {
                ret = send(fd, result.c_str(), result.length(), 0);
                if (ret == -1)
                    throw std::exception();
            }
        }
        else // the command is unknown, send something to the client
        {
            reply_str = numericReply(this, fd, "421", ERR_UNKNOWNCOMMAND(it->command));
            ret = send(fd, reply_str.c_str(), reply_str.length(), 0);
            if (ret == -1)
                throw std::exception();
        }
    }
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
                try { this->_acceptConnection(sockfd, pollfd); }
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
                catch (std::exception &e) { printError(e.what(), 1, false); }
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
