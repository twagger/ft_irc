#ifndef RETURNCODE_HPP
# define RETURNCODE_HPP

#include <iostream>
# include <string>

#define USERMODES "a = away (not active); B = bot; i = invisible; w = wallops (not active); r = restricted; o = Server operator; s = Server's notice"
#define CHANNELMODES "b = ban; i = invite only; k = channel's key; o = channel's operator rights"
#define NAMESPECIALS ";[]`_^{|}\\"

// LIST OF SERVER REPLIES

// CONNECTION REGISTRATION

	// CONNECTION REGISTRATION REPLY
	#define	RPL_WELCOME(nick, username, clientHost)			("Welcome to the Internet Relay Network " + nick + "!" + username + "@" + clientHost + "\r\n")		// 001
	#define	RPL_YOURHOST(serverName, version)				("Your host is " + serverName + ", running version " + version + "\r\n")							// 002
	#define	RPL_CREATED(date)								("This server was created " + date + "\r\n")														// 003
	#define RPL_MYINFO(serverName, version, userModes, channelModes) (serverName + " " + version + " User modes: " + userModes + " Channel modes: " + channelModes + "\r\n")				// 004
	// SERVICE MESSAGES
	#define RPL_YOURESERVICE(servicename)					("You are service " + servicename + "\r\n")															//383

	// CHANNELS
	#define RPL_TOPIC(channelName, topic)					(channelName + " :" + topic + "\r\n")																// 332
	#define RPL_NOTOPIC(channelName)						(channelName + " :No topic is set" + "\r\n")														// 331
	#define RPL_NAMREPLY(channelName, nickName) 			(channelName + " :" + nickName + "\r\n")															// 353
	#define RPL_ENDOFNAMES(channelName)						(channelName + " :End of NAMES list" + "\r\n")														// 366
	#define RPL_LIST(channelName, topic)					(channelName + " " + topic + "\r\n")																// 322
	#define RPL_LISTEND										(":End of LIST\r\n")																				// 323
	#define RPL_BANLIST(channel, nickname)					(channel + " " + nickname + "\r\n")																	// 323
	#define RPL_ENDOFBANLIST(channel)						(":End of channel ban list\r\n")																	// 368
	#define RPL_CHANNELMODEIS(channel, mode, params)		(channel + " " +  mode + " " + params + "\r\n")														// 324

	// NICK
	#define ERR_NONICKNAMEGIVEN								(":No nickname given\r\n")																	// 431
	#define ERR_ERRONEUSNICKNAME(nick)						(nick + " :Erroneous nickname" + "\r\n")													// 432
	#define	ERR_NICKNAMEINUSE(nick)							(nick + " :Nickname is already in use" + "\r\n")										// 433
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
	#define ERR_USERNOTINCHANNEL(nickname, channel)			(nickname + " " + channel + " :They aren't on that channel" + "\r\n")						// 441
	#define ERR_KEYSET(channel)								(channel + " Channel key already set" + "\r\n")												// 467
	#define ERR_UNKNOWNMODE(mode, channel)					(mode +  ":is unknown mode char to me for " + channel + "\r\n")								// 472

	// OPER
	#define RPL_YOUREOPER									(":You are now an IRC operator\r\n")														// 381
	#define ERR_PASSWDMISMATCH								(":Password incorrect\r\n")																	// 464
	#define ERR_NOOPERHOST									(":No O-lines for your host\r\n")															// 491
	
	// USER MODE
	#define RPL_UMODEIS(userModeStr) 						(userModeStr + "\r\n")																		// 221
	#define ERR_UMODEUNKNOWNFLAG 							(":Unknown MODE flag\r\n")																	// 501
	#define ERR_USERSDONTMATCH 								(":Cannot change mode for other users\r\n")													// 502

    // MOTD
    #define ERR_NOMOTD                                      (":MOTD File is missing\r\n")                                                               // 422
	#define RPL_MOTDSTART(server) 						    (":- " + server + " Message of the day - \r\n")											    // 375
	#define RPL_MOTD(text) 						            (":- " + text + "\r\n")											                            // 372
	#define RPL_ENDOFMOTD        						    (":End of MOTD command\r\n")											                    // 376

    // SERVER COMMANDS
    #define RPL_VERSION(vers, debuglvl, server, comments)   (vers + "." + debuglvl + " " + server + " :" + comments + "\r\n")                           // 351
    #define RPL_TIME(server, time)                          (server + " :" + time + "\r\n")                                                             // 391
    #define RPL_INFO(string)                                (":" + string + "\r\n")                                                                    // 371
    #define RPL_ENDOFINFO                                   (":End of INFO list\r\n")                                                                   // 374

    // SENDING MESSAGES
    #define ERR_NORECIPIENT(command)                        (":No recipient given (" + command + ")\r\n")                                               // 411
    #define ERR_NOTEXTTOSEND                                (":No text to send\r\n")                                                                    // 412
    #define ERR_NOTOPLEVEL(mask)                            (mask + " :No toplevel domain specified\r\n")                                               // 413
    #define ERR_WILDTOPLEVEL(mask)                          (mask + " :Wildcard in toplevel domain\r\n")                                                // 414
    #define ERR_CANNOTSENDTOCHAN(channame)                  (channame + " :Cannot send to channel\r\n")                                                 // 404

    // WHO
    #define RPL_WHOREPLY(channel, username, host, server, nickname, presence, star, status, realname)   (channel + " " + username + " " + host + " " + server + " " + nickname + " " + presence + star + status + " :0 " + realname + "\r\n") // 352
    #define RPL_ENDOFWHO(name)                              (name + " :End of WHO list\r\n")                                                            // 315
    
	// WHOIS
    #define RPL_WHOISUSER(nickname, username, hostname, realname) (nickname + " " + username + " " + hostname + " * :" + realname + "\r\n")             // 311
    #define RPL_WHOISCHANNELS(nickname, channelslist)       (nickname + " :" + channelslist + "\r\n")                                                   // 319
    #define RPL_WHOISIDLE(nickname, seconds)                (nickname + " " + seconds + " :seconds idle\r\n")                                           // 317
    #define RPL_WHOISSERVER(nickname, server, servinfos)    (nickname + " " + server + " :" + servinfos + "\r\n")                                       // 312
    #define RPL_WHOISOPERATOR(nickname)                     (nickname + " :is an IRC operator\r\n")                                                     // 313
    #define RPL_ENDOFWHOIS(nickname)                        (nickname + " :End of WHOIS list\r\n")                                                      // 318

	// OTHER ERRORS:
	#define	ERR_NOSUCHNICK(nickname)						(nickname + " :No such nick/channel" + "\r\n")												// 401
    #define	ERR_NOSUCHSERVER(servername)					(servername + " :No such server" + "\r\n")												    // 402
    #define ERR_NOORIGIN									(":No origin specified\r\n")																// 409
	#define	ERR_UNKNOWNCOMMAND(command)						(command + " :Unknown command" + "\r\n") 													// 421
	#define ERR_NOTREGISTERED								(":You have not registered\r\n")															// 451
	#define	ERR_NOPRIVILEGES        						(":Permission denied - You are not an IRC operator\r\n")			        	            // 481
    #define ERR_CANTKILLSERVER								(":You can't kill a server!\r\n")	                                                        // 483


	// LIST OF CLIENT ORIGINATED REPLIES

	//ADDITIONAL CLIENT REPLIES
    #define	PING(origin)                                    ("PING " + origin + "\r\n")
    #define	PONG(origin)                                    (":" + origin + " PONG " + origin + "\r\n")
	#define	PRIVMSG(target, message)						("PRIVMSG " + target + " :" + message)
	#define CLIENT_QUIT(prefix, msg)						(prefix + " " + msg + "\r\n")
	#define	CLIENT_ERROR									("ERROR : \r\n")
	#define	CLIENT_ERRORMSG(msg)							("ERROR :\"" + msg + "\"\r\n")
	#define	ERRORMSG(msg)							        ("ERROR : :" + msg + "\r\n")

#endif
