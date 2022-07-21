/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 10:19:33 by twagner           #+#    #+#             */
/*   Updated: 2022/07/20 18:10:04 by twagner          ###   ########.fr       */
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

// Channel util functions
std::vector<std::string> split_by_comma(std::string parameter);
bool                     is_channel(std::string channel_name);

#endif