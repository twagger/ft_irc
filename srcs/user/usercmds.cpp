/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usercmds.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erecuero <erecuero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 14:20:52 by erecuero          #+#    #+#             */
/*   Updated: 2022/07/20 20:34:06 by erecuero         ###   ########.fr       */
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


bool	pass(Server irc, const int &fd, std::string password) {
	if (irc.getUserByFd(fd) != 0)
	{
		if (password.empty()) {
			// reply(irc.getName(), "461", irc.getUserByFd(fd)->getNickname(), ERR_NEEDMOREPARAMS("PASS"));
			return false;
		}
		else if (irc.getUserByFd(fd)->getPassword() == true) {
			reply(irc.getName(), "462", irc.getUserByFd(fd)->getNickname(), ERR_ALREADYREGISTRED);
			return false;
		}
		else if (!irc.getUserByFd(fd)->getPassword() && irc.getPassword() == password) {	// never authenticated with a password + password given is OK
			irc.getUserByFd(fd)->setPassword(true);
			return true;
		}
	}
	return (false);
}








// OPER: SQUIT, CONNECT, KILL

