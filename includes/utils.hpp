/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 10:19:33 by twagner           #+#    #+#             */
/*   Updated: 2022/07/19 16:22:53 by twagner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
#include <vector>

// Util functions
int         print_error(std::string message, int code, bool with_errno);
std::string get_next_tokn(std::string *str, std::string delimiter);

// Channel util functions
std::vector<std::string> split_by_comma(std::string parameter);
bool                     is_channel(std::string channel_name);

#endif