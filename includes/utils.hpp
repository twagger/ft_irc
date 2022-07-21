/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erecuero <erecuero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 10:19:33 by twagner           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/07/20 18:10:04 by twagner          ###   ########.fr       */
=======
/*   Updated: 2022/07/20 20:25:32 by erecuero         ###   ########.fr       */
>>>>>>> d9287a4be26c753c7ec19168c2b9aea8e3351bd7
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include "Server.hpp"

# include <string>
# include <vector>

// Util functions
int                         printError(std::string message, int code, \
                                        bool with_errno);
std::vector<std::string>    splitBy(std::string str, std::string delimiter);
std::vector<Command>        splitCmds(std::vector<std::string> cmd_strings);
# include "replies.hpp"

// Util functions
int         print_error(std::string message, int code, bool with_errno = false);
std::string reply(std::string servername, std::string code, std::string nickname, std::string replyMsg);
std::string get_next_tokn(std::string *str, std::string delimiter);

// Channel util functions
std::vector<std::string> split_by_comma(std::string parameter);
bool                     is_channel(std::string channel_name);

#endif