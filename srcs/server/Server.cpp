// Standard headers
#include <netinet/in.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
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
#include <algorithm>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sstream>

// Custom headers
#include "../../includes/Server.hpp"
#include "../../includes/channel.hpp"
#include "../../includes/irc.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/commands.hpp"

#ifndef HOSTNAME
# define HOSTNAME "localhost"
#endif

/* ************************************************************************** */
/* Constructors & destructor                                                  */
/* ************************************************************************** */
Server::Server(int port, std::string password, std::string name)
: _port(port), _password(password), _name(name), _hostname(HOSTNAME), _version(VERSION)
{ 
	time_t      rawtime = time(NULL);
    struct tm   *timeinfo;

    timeinfo = localtime(&rawtime);
	_date = std::string(asctime(timeinfo));
	this->_initCommandList(); 
}

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

User*		Server::getUserByNickname(const std::string &nick) const
{
	std::map<int, User *>::const_iterator it;

    for (it = this->_userList.begin(); it != this->_userList.end(); ++it)
	{
        if (it->second->getNickname() == nick)
            return (it->second);
    }
    return (NULL);
}

std::deque<User*>  Server::getUsersByHostname(const std::string &hostname) const
{
    std::map<int, User *>::const_iterator   it;
    std::deque<User*>                       result;

    for (it = this->_userList.begin(); it != this->_userList.end(); ++it)
	{
        if (it->second->getHostname() == hostname)
            result.push_back(it->second);
    }
    return (result);
}

User*		Server::getUserByUsername(const std::string &user, \
                                const std::string &host) const
{
    std::map<int, User *>::const_iterator it;

    for (it = this->_userList.begin(); it != this->_userList.end(); ++it)
	{
        if (it->second->getUsername() == user)
        {
            if (!host.empty())
            {
                if (it->second->getHostname() == host)
                    return (it->second);
                else
                    return (NULL);
            }
            return (it->second);
        }
    }
    return (NULL);
}

std::deque<User*>   Server::getAllUsers(void) const
{
    std::map<const int, User *>::const_iterator it;
    std::deque<User*>                           users;

    for (it = this->_userList.begin(); it != this->_userList.end(); ++it)
        users.push_back(it->second);
    return (users);
}

/* ************************************************************************** */
/* Private member functions                                                   */
/* ************************************************************************** */
// CREATE SOCKET
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

// BIND SOCKET
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

// CREATE POLL
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
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(pollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
        throw Server::pollException();
    return (pollfd);
}

// WAIT POLL
int Server::_pollWait(int pollfd, struct epoll_event **events, int max_events)
{
    int nfds;

    nfds = epoll_wait(pollfd, *events, max_events, WAIT_TIMEOUT);
    if (nfds == -1)
        throw Server::pollWaitException();
    return (nfds);
}

// ACCEPT CONNECTION
void    Server::_acceptConnection(int sockfd, int pollfd)
{
    socklen_t           sin_size;
    struct epoll_event  ev;
	int					newfd;
    struct sockaddr_in  client_addr;


    // accept the connect request
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    memset(&sin_size, 0, sizeof(socklen_t));
    newfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&client_addr), \
        &sin_size);
    if (newfd == -1)
        throw Server::acceptException();

    // Ask getsockname to fill in this socket's local address
    getsockname(newfd, reinterpret_cast<struct sockaddr*>(&client_addr), &sin_size);
    // create a new empty user
    this->_userList[newfd] = new User(newfd, inet_ntoa(client_addr.sin_addr));

	// TO DELETE
	// hostent *host = gethostbyname(inet_ntoa(client_addr.sin_addr));
	// std::cout << "[DEBUG] sin addr " << host->h_name << std::endl;

    // add the new fd to the poll
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = newfd;
    if (epoll_ctl(pollfd, EPOLL_CTL_ADD, newfd, &ev) == -1)
        throw Server::pollAddException();
}

// HANDLE NEW MESSAGE
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
        throw Server::readException();
    buf[ret] = '\0';
	
    // split the commands in a vector. Non blocking in case of not ok message.
    try { cmd_strings = splitBy(buf, "\r\n"); } 
    catch (std::runtime_error &e) { 
        // Send an error to the client and kill the connection
        this->sendClient(event.data.fd, ERRORMSG(std::string(e.what())));
        this->killConnection(event.data.fd);
        return;
    }

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

// INIT COMMAND LIST OF THE SERVER
void    Server::_initCommandList(void) // functions to complete
{
    this->_cmdList["PASS"] = &pass;
    this->_cmdList["NICK"] = &nick;
	this->_cmdList["USER"] = &user;
	this->_cmdList["MODE"] = &mode;
	this->_cmdList["OPER"] = &oper;
    this->_cmdList["KILL"] = &kill;
    this->_cmdList["JOIN"] = &join;
    this->_cmdList["PART"] = &part;
    this->_cmdList["INVITE"] = &invite;
    this->_cmdList["KICK"] = &kick;
    this->_cmdList["TOPIC"] = &topic;
    this->_cmdList["LIST"] = &list;
    this->_cmdList["NAMES"] = &names;
    this->_cmdList["PING"] = &ping;
    this->_cmdList["PONG"] = &pong;
	this->_cmdList["QUIT"] = &quit;
    this->_cmdList["MOTD"] = &motd;
    this->_cmdList["VERSION"] = &version;
    this->_cmdList["TIME"] = &time;
    this->_cmdList["INFO"] = &info;
    this->_cmdList["PRIVMSG"] = &privmsg;
    this->_cmdList["NOTICE"] = &notice;
    this->_cmdList["CAP"] = &cap;
}

// EXECUTE RECEIVED COMMANDS
void    Server::_executeCommands(const int fd, std::vector<Command> cmds)
{
    std::vector<Command>::iterator                  it;
    std::map<std::string, CmdFunction>::iterator    it_cmd;
    std::string                                     result;
    CmdFunction                                     exec_command;
    std::string                                     reply_str;
    User                                            *user;

    // for each command in the message
    for (it = cmds.begin(); it < cmds.end(); ++it)
    {
        // search if it is in the known commands list of the server
		std::transform(it->command.begin(), it->command.end(),
				it->command.begin(), ::toupper);
		it_cmd = this->_cmdList.find(it->command);
        if (it_cmd != this->_cmdList.end())
        {
            // execute the command
            exec_command = it_cmd->second;
			user = this->getUserByFd(fd);
			// update client timers
			user->setLastActivityTime();
            if (user->getAuthenticated() || isAuthenticationCmd(it_cmd->first)) {
				try { exec_command(fd, it->params, it->prefix, this); }
				// send exception
				catch (Server::invalidFdException &e)
				{ printError(e.what(), 1, false); return; }
				catch (Server::sendException &e)
				{ printError(e.what(), 1, true); return; }
			}
        }
        else // the command is unknown, send something to the client
        {
            try { this->sendClient(fd, \
               numericReply(this, fd, "421", ERR_UNKNOWNCOMMAND(it->command)));}
            catch (Server::invalidFdException &e)
            { printError(e.what(), 1, false); return; }
            catch (Server::sendException &e)
            { printError(e.what(), 1, true); return; }
        }
    }
}

// PING CLIENTS
void    Server::_pingClients(void)
{
    int                             userFd;
    User                            *user;
    double                          seconds;
    std::map<int, User *>::iterator it;
    std::stringstream               ss;

    // loop on every active connection
    for (it = this->_userList.begin(); it != this->_userList.end();)
    {
        userFd = it->first;
        user = it->second;
        // Check user's last activity time 
        seconds = difftime(time(NULL), user->getLastActivityTime());
        // case 1 : send a PING command if > 120sec
        if (user->getStatus() == ST_ALIVE && seconds > PING_TIMEOUT)
        {
            try { this->sendClient(userFd, PING(this->_hostname)); }
            catch (Server::sendException &e)
            { printError(e.what(), 1, true); return; }
            catch (Server::invalidFdException &e)
            { printError(e.what(), 1, false); return; }
            user->setStatus(ST_PINGED);
            ++it;
        }
        else if (user->getStatus() == ST_PINGED) 
        {
            seconds = difftime(time(NULL), user->getPingTime());
            // case 2 : PONG timeout > Kill the client
            if (seconds > PONG_TIMEOUT)
            {
                // Move the iterator to the next user before removing user 
                ++it;
                // Send en error to the client
                ss << PONG_TIMEOUT;
                this->sendClient(userFd, ERRORMSG(std::string("Ping timeout: ")
                                        .append(ss.str()).append(" seconds")));
                // Kill connection
                this->killConnection(userFd);
            }
        }
        else
            ++it;
    }
}

/* ************************************************************************** */
/* Member functions                                                           */
/* ************************************************************************** */
// START
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
    this->_sockfd = sockfd;

    // binding to ip address + port and switch to passive mode --------------- /
    try { this->_bindSocket(sockfd, &srv_addr); }
    catch (Server::bindException &e) { printError(e.what(), 1, true); return; }

    // poll creation --------------------------------------------------------- /
    try { pollfd = this->_createPoll(sockfd); }
    catch (Server::pollException &e) { printError(e.what(), 1, true); return; }
    this->_pollfd = pollfd;

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
                catch (Server::readException &e)
                { printError(e.what(), 1, true); }
            }
        }

        // send a PING to fds that seems inactive ---------------------------- /
        try { this->_pingClients(); }
        catch (Server::pollDelException &e)
        { printError(e.what(), 1, true); return; }
    }
}

// KILL CONNECTION

void	cleanFd(Server *srv, User *user) {

	std::map<std::string, Channel *>::iterator chanIt = srv->_channelList.begin();
	std::map<std::string, Channel *>::iterator chanIte = srv->_channelList.end();
	for (; chanIt != chanIte; chanIt++) {
		std::deque<User*>::iterator userIt = ((chanIt->second)->_users).begin();
		std::deque<User*>::iterator userIte = ((chanIt->second)->_users).end();
		for (; userIt != userIte; userIt++) {
			if (user == *userIt)
				((chanIt->second)->_users).erase(userIt);
		}
	}
}

void    Server::killConnection(const int &fd)
{
    std::map<int, User *>::iterator it;

    // check if fd/user exists using the userlist ---------------------------- /
    it = this->_userList.find(fd);
    if (it == this->_userList.end()) {
        throw Server::invalidFdException();
	}

    // fd exists, clean all -------------------------------------------------- /
    // delete user and remove pair from list  -------------------------------- /
	cleanFd(this, it->second);
	delete it->second;
	this->_userList.erase(fd);

    // remove user's fd from the poll  --------------------------------------- /
    if (epoll_ctl(this->_pollfd, EPOLL_CTL_DEL, fd, NULL) == -1)
        throw Server::pollDelException();
    // close the socket  ----------------------------------------------------- /
    close(fd);
}

// SEND CLIENT (ONE FD)
void    Server::sendClient(const int &fd, const std::string &message, \
                           const int &originFd) const
{
    if (originFd != -1 && originFd == fd)
        return;
    if (this->_userList.find(fd) == this->_userList.end())
        throw Server::invalidFdException();
    if (send(fd, message.c_str(), message.length(), MSG_NOSIGNAL) == -1)
        throw Server::sendException();
}

// SEND CLIENT (MULTIPLE FDS)
void    Server::sendClient(const std::set<int> &fds, \
                           const std::string &message, \
                           const int &originFd) const
{
    std::set<int>::const_iterator it;

    for (it = fds.begin(); it != fds.end(); ++it)
        this->sendClient(*it, message, originFd);
}

// SEND CHANNEL
void    Server::sendChannel(const std::string &channel, \
                            const std::string &message, \
                            const int &originFd) const
{
    std::map<std::string, Channel *>::const_iterator    itChannel;
    std::deque<User *>::const_iterator                  itUsers;
    std::deque<User *>                                  userList;

    itChannel = this->_channelList.find(channel);
    if (itChannel == this->_channelList.end())
        throw Server::invalidChannelException();
    userList = itChannel->second->getUsers();
    for (itUsers = userList.begin(); itUsers != userList.end(); ++itUsers) {
		if (((*itUsers)->getFd() != 0)) {
        	this->sendClient((*itUsers)->getFd(), message, originFd);
		}
	}
}

// BROADCAST
void    Server::broadcast(const std::string &message,const int &originFd) const
{
    std::map<const int, User *>::const_iterator it;
    std::set<int>                               fds;

    for (it = this->_userList.begin(); it != this->_userList.end(); ++it)
    {
        fds.insert(it->first);
        this->sendClient(fds, message, originFd);
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

const char	*Server::pollDelException::what() const throw()
{ return ("Poll del error: "); }

const char	*Server::acceptException::what() const throw()
{ return ("Accept error: "); }

const char	*Server::passwordException::what() const throw()
{ return ("Password error: please provide a correct password"); }

const char	*Server::invalidFdException::what() const throw()
{ return ("Fd error: please provide a valid fd"); }

const char	*Server::invalidChannelException::what() const throw()
{ return ("Invalid Channel: please provide a valid channel"); }

const char	*Server::sendException::what() const throw()
{ return ("Send error: "); }

const char	*Server::readException::what() const throw()
{ return ("Read error: "); }
