/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erecuero <erecuero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 12:58:05 by erecuero          #+#    #+#             */
/*   Updated: 2022/07/20 15:38:07 by erecuero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/user.hpp"

// CONSTRUCTORS
User::User(int fd) : _fd(fd), _mode(0), _authentified(false) { }

User::User(const User &src) {
	*this = src;
}

// DESTRUCTORS
User::~User() { }

// ASSIGN OVERLOAD
User& User::operator=(User const &rhs) {
	if (this != &rhs) {
		this->_fd = rhs._fd;
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
		this->_fullname = rhs._fullname;
		this->_mode = rhs._mode;
		this->_password = rhs._password;
		this->_authentified = rhs._authentified;
	}
	return *this;
}

// GETTERS
std::string	User::getNickname(void) const { return this->_nickname; }
std::string	User::getUsername(void) const { return this->_username; }
std::string	User::getFullname(void) const { return this->_fullname; }
uint16_t	User::getMode(void) const { return this->_mode; }
bool 		User::getPassword(void) const { return this->_password; }
bool 		User::getAuthentified(void) const { return this->_authentified; }

// SETTERS
void User::setNickname(std::string nickname) { this->_nickname = nickname; }
void User::setUsername(std::string username) { this->_username = username; }
void User::setFullname(std::string fullname) { this->_fullname = fullname; }
void User::setMode(uint16_t mode) { this->_mode = mode; }
void User::setPassword(bool password) { this->_password = password; }
void User::setAuthentified(bool authentified) { this->_authentified = authentified; }

// OSTREAM 
std::ostream & operator<<(std::ostream &o, User const &rhs) {
	if (rhs.getAuthentified() == true)
		o << rhs.getNickname() << "!" << rhs.getUsername();
	return o;
}
