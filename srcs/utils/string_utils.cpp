/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 16:06:53 by twagner           #+#    #+#             */
/*   Updated: 2022/07/20 18:44:31 by twagner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

// Split a string by \n\r and return a string vector
std::vector<std::string>    splitBy(std::string str, std::string delimiter)
{
    std::vector<std::string>    result;
    int                         end;

    // first check 
    std::cout << "received : " << str << std::endl;
    end = str.find(delimiter);
    if (end == -1)
        throw std::runtime_error("IRC message must end with CRLF");
    // save first command in vector
    result.push_back(str.substr(0, end));
    // update str
    str.erase(0, end + delimiter.length());
    // loop for other commands
    end = str.find(delimiter);
    while (end != -1)
    {
        result.push_back(str.substr(0, end));
        str.erase(0, end + delimiter.length());
        end = str.find(delimiter);
    }
    return (result);
}
