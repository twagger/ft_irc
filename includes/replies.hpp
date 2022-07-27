#ifndef RETURNCODE_HPP
# define RETURNCODE_HPP

#include <iostream>
# include <string>

#define USERMODES "iorx"
#define CHANNELMODES ""
#define NAMESPECIALS ";[]`_^{|}\\"

// LIST OF SERVER REPLIES
// find them here: https://datatracker.ietf.org/doc/html/rfc2812#section-5.1

// USE AS: numericReply(irc, fd, "461", ERR_NEEDMOREPARAMS(std::string("CMD")));

// CONNECTION REGISTRATION

	// CONNECTION REGISTRATION REPLY
	#define	RPL_WELCOME(nick, username, clientHost)				("Welcome to the Internet Relay Network " + nick + "!" + username + "@" + clientHost + "\r\n")	// 001
	#define	RPL_YOURHOST(serverName, version)				("Your host is " + serverName + ", running version " + version + "\r\n")					// 002
	#define	RPL_CREATED(date)								("This server was created " + date + "\r\n")												// 003
	#define RPL_TOPIC(channelName, topic)					(channelName + " :" + topic)																// 332
	#define RPL_NOTOPIC(channelName)						(channelName + " :No topic is set")															// 331
	#define RPL_NAMREPLY(channelName, nickname)																											// 353
	#define RPL_ENDOFNAMES(channelName)						(channelName + " :End of NAMES list")														// 366

	#define RPL_MYINFO(serverName, version, userModes, channelModes) (serverName + " " + version + " " + userModes + " " + channelModes)				// 004

	// NICK
	#define ERR_NONICKNAMEGIVEN								(":No nickname given\r\n")																	// 431
	#define ERR_ERRONEUSNICKNAME(nick)						(nick + " :Erroneous nickname" + "\r\n")													// 432
	#define	ERR_NICKNAMEINUSE(nick)							(nick + " :Nickname is already in use" + "\r\n")											// 433
	//#define ERR_NICKCOLLISION(nick, user, clientHost) 		(nick + " :Nickname collision KILL from " + user + "@" + clientHost + "\r\n")				// 436
	#define ERR_UNAVAILRESOURCE(nickOrChannel) 				(nickOrChannel + " :Nick/channel is temporarily unavailable" + "\r\n")						// 437
	#define ERR_RESTRICTED 									(":Your connection is restricted!\r\n")														// 484

	// PASS
	#define ERR_NEEDMOREPARAMS(command)						(command + " :Not enough parameters" + "\r\n")												// 461
	#define ERR_ALREADYREGISTRED 							(":Unauthorized command (already registered)\r\n")											// 462

	// CHANNEL CONNECTION

	#define ERR_NOSUCHCHANNEL(channelName)					(channelName + " :No such channel" + "\r\n")												// 403
	#define ERR_TOOMANYCHANNELS(channelName)				(channelName + " :You have joined too many channels" + "\r\n")								// 405
	#define ERR_TOOMANYTARGETS(target, errorCode, AbortMsg)	(target + " :" + errorCode + " recipients. "+ AbortMsg + "\r\n")							// 407
	#define ERR_CHANNELISFULL(channel)						(channel + " :Cannot join channel (+l)" + "\r\n")											// 471
	#define ERR_INVITEONLYCHAN(channel)						(channel + " :Cannot join channel (+i)" + "\r\n")											// 473
	#define ERR_BANNEDFROMCHAN(channel) 					(channel + " :Cannot join channel (+b)" + "\r\n")											// 474
	#define ERR_BADCHANNELKEY(channel) 						(channel + " :Cannot join channel (+k)" + "\r\n")											// 475
	#define ERR_BADCHANMASK(channel) 						(channel + " :Bad Channel Mask" + "\r\n")													// 476
	#define ERR_NOTONCHANNEL(channel)						(channel + " :You're not on that channel" + "\r\n")											// 442
	#define ERR_USERONCHANNEL(user, channel)				(user + " " + channel + " :is already on channel" + "\r\n")									// 443
	#define ERR_CHANOPRIVSNEEDED(channel)					(channel + " :You're not channel operator" + "\r\n")										// 482

	// USER
	// #define ERR_NEEDMOREPARAMS 461
	// #define  ERR_ALREADYREGISTRED 462

	// OPER
	#define RPL_YOUREOPER									(":You are now an IRC operator\r\n")														// 381
	#define ERR_PASSWDMISMATCH								(":Password incorrect\r\n")																	// 464
	#define ERR_NOOPERHOST									(":No O-lines for your host\r\n")															// 491
	// #define ERR_NEEDMOREPARAMS 461

	// USER MODE
	#define RPL_UMODEIS(userModeStr) 						(userModeStr + "\r\n")																		// 221
	#define ERR_USERSDONTMATCH 								(":Cannot change mode for other users\r\n")													// 502
	#define ERR_UMODEUNKNOWNFLAG 							(":Unknown MODE flag\r\n")																	// 501
	// #define ERR_NEEDMOREPARAMS 461


	// OTHER ERRORS:
	#define	ERR_NOPRIVILEGES(nickname)						(nickname + " :Permission denied - You are not an IRC operator")							// 481
	#define	ERR_NOSUCHNICK(nickname)						(nickname + " :No such nick/channel")														// 401
    #define	ERR_NOSUCHSERVER(servername)					(servername + " :No such server"+ "\r\n")												    // 402
	#define	ERR_UNKNOWNCOMMAND(command)						(command + " :Unknown command") 															// 421
	#define ERR_NOTREGISTERED								(":You have not registered\r\n")															// 451
    #define ERR_NOORIGIN									(":No origin specified\r\n")																// 409


	// LIST OF CLIENT ORIGINATED REPLIES

	// Use as: clientReply()

	//ADDITIONAL CLIENT REPLIES
	#define CLIENT_NICK(nickname)							("NICK " + nickname + "\r\n")

#endif
