#ifndef RETURNCODE_HPP
# define RETURNCODE_HPP

#include <iostream>
# include <string>
// LIST OF ERRORS
// find them here: https://datatracker.ietf.org/doc/html/rfc2812#section-5.1

// CONNECTION REGISTRATION

	// REPLY
	#define	RPL_WELCOME(nick, user, clientHost)				("Welcome to the Internet Relay Network " + nick + "!" + user + "@" + clientHost + "\r\n")	// 001
	#define	RPL_YOURHOST(serverName, version)				("Your host is " + serverName + ", running version " + version + "\r\n")						// 002
	#define	RPL_CREATED(date)								("This server was created " + date + "\r\n")													// 003

	#define ERR_NOTREGISTERED								(":You have not registered" + "\r\n")														// 451

	// NICK
	#define ERR_NONICKNAMEGIVEN								(":No nickname given" + "\r\n")																// 431
	#define ERR_ERRONEUSNICKNAME(nick)						(nick + " :Erroneous nickname" + "\r\n")														// 432
	#define	ERR_NICKNAMEINUSE(nick)							(nick + " :Nickname is already in use" + "\r\n")												// 433
	#define ERR_NICKCOLLISION(nick, user, clientHost) 		(nick + " :Nickname collision KILL from " + user + "@" + clientHost + "\r\n")				// 436
	#define ERR_UNAVAILRESOURCE(nickChannel) 				(nickChannel + " :Nick/channel is temporarily unavailable" + "\r\n")							// 437
	#define ERR_RESTRICTED 									(":Your connection is restricted!\r\n")													// 484

	// PASS
	#define ERR_NEEDMOREPARAMS(command)						(#command + " :Not enough parameters" + "\r\n")												// 461
	#define ERR_ALREADYREGISTRED 							(":Unauthorized command (already registered)\r\n")										// 462

// CHANNEL CONNECTION

	#define ERR_NOSUCHCHANNEL(channelName)					(channelName + " :No such channel" + "\r\n")													// 403
	#define ERR_TOOMANYCHANNELS(channelName)				(channelName + " :You have joined too many channels" + "\r\n")								// 405
	#define ERR_TOOMANYTARGETS(target, errorCode, AbortMsg)	(target + " :" + errorCode + " recipients. "+ AbortMsg + "\r\n")								// 407
	#define ERR_CHANNELISFULL(channel)						(channel + " :Cannot join channel (+l)" + "\r\n")											// 471
	#define ERR_INVITEONLYCHAN(channel)						(channel + " :Cannot join channel (+i)" + "\r\n")											// 473
	#define ERR_BANNEDFROMCHAN 								(channel + " :Cannot join channel (+b)" + "\r\n")											// 474
	#define ERR_BADCHANNELKEY 								(channel + " :Cannot join channel (+k)" + "\r\n")											// 475
	#define ERR_BADCHANMASK 								(channel + " :Bad Channel Mask" + "\r\n")													// 476
	// #define ERR_NEEDMOREPARAMS 461

	// USER
	// #define ERR_NEEDMOREPARAMS 461
	// #define  ERR_ALREADYREGISTRED 462

	// OPER
	#define RPL_YOUREOPER									(":You are now an IRC operator" + "\r\n")													// 381
	#define ERR_PASSWDMISMATCH								(":Password incorrect" + "\r\n")																// 464
	#define ERR_NOOPERHOST									(":No O-lines for your host" + "\r\n")														// 491
	// #define ERR_NEEDMOREPARAMS 461

	// USER MODE
	#define RPL_UMODEIS(userModeStr) 						(userModeStr + "\r\n")																		// 221
	#define ERR_USERSDONTMATCH 								(":Cannot change mode for other users" + "\r\n")												// 502
	#define ERR_UMODEUNKNOWNFLAG 							(":Unknown MODE flag" + "\r\n")																// 501
	// #define ERR_NEEDMOREPARAMS 461

#endif