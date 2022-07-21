/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erecuero <erecuero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 12:58:15 by erecuero          #+#    #+#             */
/*   Updated: 2022/07/21 14:51:34 by erecuero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <iostream>
# include <stdint.h>

class User {

	private:
		const int			_fd;
		std::string	_nickname;			// max 9 characters
		std::string	_username;			
		std::string _fullname;
		std::string	_hostname;
		int			_numberOfChannelJoined;
		uint16_t	_mode;				// OPER: SQUIT, CONNECT, KILL
		bool		_password;
		bool		_authenticated;

	public:
		User(const int fd, std::string hostname);
		User(const User &src);
		~User();

		User &operator=(User const &rhs);

		std::string getNickname(void) const;
		std::string getUsername(void) const;
		std::string getFullname(void) const;
		std::string getHostname(void) const;
		uint16_t 	getMode(void) const;
		bool 		getPassword(void) const;
		bool 		getAuthenticated(void) const;

		void setNickname(std::string nickname);
		void setUsername(std::string username);
		void setFullname(std::string fullname);
		void setHostname(std::string fullname);
		void setMode(uint16_t mode);
		void setPassword(bool pass);
		void setAuthenticated(bool authenticated);
};

std::ostream & operator<<(std::ostream &o, User const &rhs);	// for printing 'nickname!username@host.name.com ...'

#endif