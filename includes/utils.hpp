/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erecuero <erecuero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 10:19:33 by twagner           #+#    #+#             */
/*   Updated: 2022/07/21 12:32:55 by erecuero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>
# include "replies.hpp"

// Util functions
int         print_error(std::string message, int code, bool with_errno);
std::string reply(Server *irc, const int &fd, std::string code, std::string replyMsg);
std::string get_next_tokn(std::string *str, std::string delimiter);

// Channel util functions
std::vector<std::string> split_by_comma(std::string parameter);
bool                     is_channel(std::string channel_name);

#endif