/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twagner <twagner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 10:19:22 by twagner           #+#    #+#             */
/*   Updated: 2022/07/20 17:52:53 by twagner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"
#include <string>
#include <cstring>
#include <iostream>
#include <cerrno>

int printError(std::string message, int code, bool with_errno = false)
{
    std::cout << message;
    if (with_errno)
        std::cout << std::strerror(errno);
    std::cout << std::endl;
    return (code);
}
