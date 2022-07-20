/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 07:46:38 by codespace         #+#    #+#             */
/*   Updated: 2022/07/20 18:39:49 by twagner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <map>
#include "channel.hpp"
#include "user.hpp"

struct Command
{
    std::string                 command;
    std::vector<std::string>    params;

    Command(std::string cmd, \
            std::vector<std::string> params = std::vector<std::string>());
    ~Command(){};
};

class Server
{
    public:
        // Constructors & destructor
        Server(int port, std::string password, std::string name = "Gunther");
        Server(Server const &src);
        ~Server(void){};

        // Operator overload
        Server  &operator=(Server const &rhs);
        
        // Getters
        int         getPort(void) const;
        std::string getPassword(void) const;
        std::string getName(void) const;

        // Member functions
        void    start(void);
        void    stop(void); // quit all clients with a message

        // exceptions
        class socketException : public std::exception
        { public: virtual const char *what() const throw(); };
        
        class bindException : public std::exception
        { public: virtual const char *what() const throw(); };

        class pollException : public std::exception
        { public: virtual const char *what() const throw(); };

        class pollWaitException : public std::exception
        { public: virtual const char *what() const throw(); };

        class pollAddException : public std::exception
        { public: virtual const char *what() const throw(); };

        class acceptException : public std::exception
        { public: virtual const char *what() const throw(); };
        
        class passwordException : public std::exception
        { public: virtual const char *what() const throw(); };

    private:
        // Cannot be default construct
        Server(void){};

        // Private member functions
        int     _createSocket(void);
        void    _bindSocket(int sockfd, struct sockaddr_in *srv_addr);
        int     _createPoll(int sockfd);
        int     _pollWait(int pollfd, struct epoll_event **events, \
                           int max_events);
        void    _acceptConnection(int sockfd, int pollfd, \
                                   struct sockaddr_in *srv_addr);
        void    _handleNewMessage(struct epoll_event event);

        // Member attributes
        int         _port;
        std::string _password;
        std::string _name;
        
        std::map<int, User *>               _userList;
        std::map<std::string, Channel *>    _channelList;
        std::map<std::string, void *>       _cmdList;
};

#endif
