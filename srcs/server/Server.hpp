/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 07:46:38 by codespace         #+#    #+#             */
/*   Updated: 2022/07/20 09:58:01 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include "User.hpp"

class Server
{
    public:
        // Constructors & destructor
        Server(int port, std::string password, std::string name = "Gunther")
        : _port(port), _password(password), _name(name)
        Server(Server const &src);
        ~Server(void);

        // Operator overload
        Server  &operator=(Server const &rhs);
        
        // Getters
        int         &getPort(void) const;
        std::string &getPassword(void) const;
        std::string &getName(void) const;

        // Member functions
        start(void); // create socket then listen
        stop(void); // quit all client with a message
        int add_user(void);
        int del_user(void);

    private:
        // Cannot be default construct
        Server(void);

        // Member attributes
        int                         _port;
        std::string                 _password            
        const std::string           _name;
        std::vector<User *>         _user_list;
        map<std::string, void *>    _cmd_list;
}

#endif