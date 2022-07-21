/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usercmds.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erecuero <erecuero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 14:20:52 by erecuero          #+#    #+#             */
/*   Updated: 2022/07/21 13:00:14 by erecuero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "usercmds.hpp"
#include "../server/Server.hpp"
#include "utils.hpp"


// Registration functions for each client

// order:
// 1. Pass message
// 2. Nick message
// 3. User message

// replies:
// RPL_WELCOME

// PASS MUST precede NICK/USER combination for user connection

// rules for nickname / username:
// user       =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF )
//                  ; any octet except NUL, CR, LF, " " and "@"


std::string	pass(Server *irc, const int &fd, std::vector<std::string> params) {
	if (irc->getUserByFd(fd) != 0)
	{
		if (params[0].empty())
			return reply(irc, fd, "461", ERR_NEEDMOREPARAMS(std::string("PASS")));
		else if (irc->getUserByFd(fd)->getPassword() == true)
			return reply(irc, fd, "462", ERR_ALREADYREGISTRED);
		else if (!irc->getUserByFd(fd)->getPassword() && irc->getPassword() == params[0]) 
		{	
			// never authenticated with a password + password given is OK
			irc->getUserByFd(fd)->setPassword(true);
			return (NULL);
		}
	}
	// if wrong password but not authenticated, do nothing >> TO BE CHECKED
	// if user not find, will return null as well
	return (NULL);
}


std::string nick(Server *irc, const int &fd, std::vector<std::string> params) {
	if (irc->getUserByFd(fd) != 0)
	{
		if ()
	}
	return (NULL);
}






// OPER: SQUIT, CONNECT, KILL

