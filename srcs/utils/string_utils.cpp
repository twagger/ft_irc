/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 16:06:53 by twagner           #+#    #+#             */
/*   Updated: 2022/07/19 16:22:41 by twagner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

std::string get_next_tokn(std::string *str, std::string delimiter)
{
    std::string token;
    int         end;
    
    end = (*str).find(delimiter);
    if (end == -1)
        end = (*str).length();
    token = (*str).substr(0, end);
    (*str).erase(0, end + delimiter.length());
    return (token);
}
