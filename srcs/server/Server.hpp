/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 07:46:38 by codespace         #+#    #+#             */
/*   Updated: 2022/07/20 15:24:59 by twagner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>

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
        class SocketException : public std::exception
        { public: virtual const char *what() const throw(); };
        
        class BindException : public std::exception
        { public: virtual const char *what() const throw(); };

        class PollException : public std::exception
        { public: virtual const char *what() const throw(); };

        class PollWaitException : public std::exception
        { public: virtual const char *what() const throw(); };

        class PollAddException : public std::exception
        { public: virtual const char *what() const throw(); };

        class AcceptException : public std::exception
        { public: virtual const char *what() const throw(); };

    private:
        // Cannot be default construct
        Server(void){};

        // Private member functions
        int     _create_socket(void);
        void    _bind_socket(int sockfd, struct sockaddr_in *srv_addr);
        int     _create_poll(int sockfd);
        int     _poll_wait(int pollfd, struct epoll_event **events, \
                           int max_events);
        void    _accept_connection(int sockfd, int pollfd, \
                                   struct sockaddr_in *srv_addr);

        // Member attributes
        int         _port;
        std::string _password;
        std::string _name;
        
        // std::vector<User *>             _user_list;
        // std::vector<Channel *>          _channel_list;
        // std::map<std::string, void *>   _cmd_list;
};

#endif