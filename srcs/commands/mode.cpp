#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/parser.hpp"

/* ************************************************************************** */
/* USER		                                                                  */
/* ************************************************************************** */

std::string userModesToStr(User *user)
{
	std::string modes;

	modes.append("+");
	if (user->hasMode(MOD_AWAY))
		modes.append("a");
	if (user->hasMode(MOD_BOT))
		modes.append("B");
	if (user->hasMode(MOD_WALLOPS))
		modes.append("w");
	if (user->hasMode(MOD_INVISIBLE))
		modes.append("i");
	if (user->hasMode(MOD_OPER))
		modes.append("o");
	if (user->hasMode(MOD_SRVNOTICES))
		modes.append("s");
	if (user->hasMode(MOD_RESTRICTED))
		modes.append("r");
	return (modes);
}

void addModes(User *user, const std::string mode, int start, int stop)
{
	for (int i = start; i < stop; i++)
	{
		switch (mode[i])
		{
			case 'a':
				break;
			case 'w':
				user->addMode(MOD_WALLOPS);
				break;
			case 'i':
				user->addMode(MOD_INVISIBLE);
				break;
			case 'o':
				break;
			case 's':
				user->addMode(MOD_SRVNOTICES);
				break;
			case 'r':
				user->addMode(MOD_RESTRICTED);
				user->removeMode(MOD_OPER);
				break;
		}
	}
}

void removeModes(User *user, const std::string mode, int start, int stop)
{
	for (int i = start; i < stop; i++)
	{
		switch (mode[i])
		{
			case 'a':
				break;
			case 'w':
				user->removeMode(MOD_WALLOPS);
				break;
			case 'i':
				user->removeMode(MOD_INVISIBLE);
				break;
			case 'o':
				user->removeMode(MOD_OPER);
				break;
			case 's':
				user->removeMode(MOD_SRVNOTICES);
				break;
			case 'r':
				break;
		}
	}
}

void findPair(const std::string modes, unsigned int i, std::pair<char, int> *pair)
{
	pair->first = modes[i];
	pair->second = i;

	for (i = i + 1; i < modes.size(); i++)
	{
		if (modes[i] == '+' || modes[i] == '-')
			break;
		else
			pair->second++;
	}
	return;
}

void handleAddRemoveModes(User *user, const std::string modes)
{
	std::pair<char, int> pair;

	for (unsigned int i = 0; i < modes.size(); i++)
	{
		if (modes[i] == '+' || modes[i] == '-')
			findPair(modes, i, &pair);
		if (modes[i] == '+' && pair.second > 0)
			addModes(user, modes, i, pair.second + 1);
		else if (modes[i] == '-' && pair.second > 0)
			removeModes(user, modes, i, pair.second + 1);
	}
}

void UserMode(const int &fd, const std::vector<std::string> &params, Server *srv)
{
	User *user = srv->getUserByFd(fd);

	if (params.size() == 1 && params[0] == user->getNickname())
		return (srv->sendClient(fd, numericReply(srv, fd, "221",
												 RPL_UMODEIS(userModesToStr(user)))));
	else if (srv->getUserByNickname(params[0]) == 0)
		return (srv->sendClient(fd, numericReply(srv, fd, "401",
												 ERR_NOSUCHNICK(params[0]))));
	else if (params[0] != user->getNickname())
		return (srv->sendClient(fd, numericReply(srv, fd, "502",
												 ERR_USERSDONTMATCH)));
	else if (params[1].find_first_not_of("+-aBiwros") != std::string::npos)
		return (srv->sendClient(fd, numericReply(srv, fd, "501",
												 ERR_UMODEUNKNOWNFLAG)));
	else if (!params[1].empty())
		handleAddRemoveModes(user, params[1]);
}

/* ************************************************************************** */
/* CHANNEL	                                                                  */
/* ************************************************************************** */

/**
 * @brief Add or remove a channel mode
 * 
 * Mode supported:
 * - i = set the channel to invite only. Users won't be able to join an existing channel
 * 		if they are not invited by an operator on the channel
 * - b = ban an existing user on the channel. The user will be added to the ban list.
 * 		He won't be able to join unless the ban is removed
 * - o = give to a user the operator rights. This flag can only be set by an operator.
 * - k = add or remove a key from the channel.
 * 		Users won't be able to join the channel without this key
 *   
 * Errors handled:
 * - ERR_NEEDMOREPARAMS
 * - ERR_USERNOTINCHANNEL
 * - ERR_KEYSET
 * - ERR_NOSUCHCHANNEL
 * - ERR_CHANOPRIVSNEEDED
 * - ERR_UNKNOWNMODE
 *   
 */

void listBannedUser(const int &fdUser, Server *server,
              Channel *channel)
{
    std::string nicknameList;
    std::deque<std::string> listBannedUser = channel->_bannedUsers;
    std::deque<std::string>::iterator itBannedUser;

    for (itBannedUser = listBannedUser.begin(); itBannedUser != listBannedUser.end();
		itBannedUser++)
        nicknameList += *itBannedUser + "!*@* ";
    server->sendClient(fdUser, numericReply(server, fdUser,
        "367", RPL_BANLIST(channel->getChannelName(), nicknameList)));
    server->sendClient(fdUser, numericReply(server, fdUser,
		"368", RPL_ENDOFBANLIST(channel->getChannelName())));
}

int checkUserExists(User *user, const std::vector<std::string> params,
	const int &fd, Server *srv, int bannedList)
{
	if (user == NULL && params.size() > 2)
	{
		srv->sendClient(fd, numericReply(srv, fd, "441",
			ERR_USERNOTINCHANNEL(params[2], params[0])));
		return (-1);
	}
	else if (params.size() < 3 && bannedList == 0)
		return (-2);
	else if (params.size() < 3 && bannedList == 1)
	{
		listBannedUser(fd, srv, srv->_channelList.find(params[0])->second);
		return (-3);
	}
	return (0);
}

void handleChannelReply(const std::vector<std::string> params, const int &fd, Server *srv)
{
	std::string channel = params[0];
	std::string mode = params[1];
	std::string user;

	if (params.size() > 2)
		std::string user = params[2];
	// Case where mode is only +i
	if (params.size() < 3)
		return(srv->sendChannel(channel, clientReply(srv, fd, "MODE " +
			channel + " " + mode)));
	// Case where mode is ban with a user as a third parameter
	if (mode.find('b') != std::string::npos
		&& user.find('*') == std::string::npos)
		return(srv->sendChannel(channel, clientReply(srv, fd, "MODE " +
			channel + " " + mode + " " + user + "!*@*")));
	// Case where mode is operator
	if (mode.find('o') != std::string::npos)
		return(srv->sendChannel(channel, clientReply(srv, fd, "MODE " +
			channel + " " + mode + " " + params[2])));
	// Case where mode is key or ban with *
	return(srv->sendChannel(channel, clientReply(srv, fd, "MODE " +
		channel + " " + mode + " " + user)));
}

void addModesChannel(const std::vector<std::string> params, int start, int stop,
					 const int &fd, Server *srv)
{
	Channel *channel = srv->_channelList.find(params[0])->second;
	User *user = NULL;
	
	if (params.size() > 2)
		user = srv->getUserByNickname(params[2]);

	for (int i = start; i < stop; i++)
	{
		switch (params[1][i])
		{
		case 'i':
			channel->addMode(MOD_INVITE);
			break ;
		case 'k':
		// Check that a key was not already set
			if (channel->hasMode(MOD_KEY) == true)
				return (srv->sendClient(fd, numericReply(srv, fd, "467",
					ERR_KEYSET(params[0]))));
			channel->addMode(MOD_KEY);
			channel->setKey(params[2]);
			break ;
		case 'o':
			// Check that the user exists and a user was given in parameter
			if (checkUserExists(user, params, fd, srv, 0) < 0)
				return ;
			channel->addMode(MOD_OPERATOR);
			channel->addOperator(user);
			break ;
		case 'b':
			// Check that the user exists and a user was given in parameter
			if (checkUserExists(user, params, fd, srv, 1) < 0)
				return ;
			channel->addBannedUser(user->getNickname());
			channel->addMode(MOD_BAN);
			break ;
		default:
			return (srv->sendClient(fd, numericReply(srv, fd, "472",
				ERR_UNKNOWNMODE(params[1], params[0]))));
		}
	}
	handleChannelReply(params, fd, srv);
}

void removeModesChannel(const std::vector<std::string> params, int start, int stop,
						const int &fd, Server *srv)
{
	Channel *channel = srv->_channelList.find(params[0])->second;
	User	*user = NULL;
	 
	 if (params.size() > 2 && params[2].find('*') == std::string::npos)
		user = srv->getUserByNickname(params[2]);
	for (int i = start; i < stop; i++)
	{
		switch (params[1][i])
		{
		case 'i':
			channel->removeMode(MOD_INVITE);
			channel->_invitees.clear();
			break ;
		case 'k':
			channel->removeMode(MOD_KEY);
			channel->setKey("");
			break ;
		case 'o':
			if (checkUserExists(user, params, fd, srv, 0) < 0)
				return ;
			channel->removeOperator(user);
			channel->removeMode(MOD_OPERATOR);
			break ;
		case 'b':
			if (checkUserExists(user, params, fd, srv, 1) < 0)
				return ;
			channel->removeBannedUser(user->getNickname());
			channel->removeMode(MOD_BAN);
			break ;
		default:
			return (srv->sendClient(fd, numericReply(srv, fd, "472",
				ERR_UNKNOWNMODE(params[1], params[0]))));
		}
	}
	handleChannelReply(params, fd, srv);
}

void handleAddRemoveModesChannel(const int &fd, const std::vector<std::string> params,
								 Server *srv)
{
	std::pair<char, int> pair;
	std::string mode = params[1];

	for (unsigned int i = 0; i < params[1].size(); i++)
	{
		if (mode[i] == '+' || mode[i] == '-')
		{
			findPair(params[1], i, &pair);
		}
		if (mode[i] == '+' && pair.second > 0)
		{
			addModesChannel(params, i + 1, pair.second + 1, fd, srv);
		}
		else if (mode[i] == '-' && pair.second > 0)
		{
			removeModesChannel(params, i + 1, pair.second + 1, fd, srv);
		}
	}
}

int checkChannelMode(const int &fd, const std::vector<std::string> &params, Server *srv)
{
	std::map<std::string, Channel *>::iterator itChannel;

	// Check that channel list is not empty
	if (srv->_channelList.empty() == true)
	{
		srv->sendClient(fd, numericReply(srv, fd, "403",
										 ERR_NOSUCHCHANNEL(params[0])));
		return (-1);
	}
	itChannel = srv->_channelList.find(params[0]);
	// Check that channel exists
	if (itChannel == srv->_channelList.end())
	{
		srv->sendClient(fd, numericReply(srv, fd, "403",
										 ERR_NOSUCHCHANNEL(params[0])));
		return (-2);
	}
	// Check that user is on channel
	if (findUserOnChannel(itChannel->second->_users, srv->getUserByFd(fd)) == false)
	{
		srv->sendClient(fd, numericReply(srv, fd, "441",
										 ERR_USERNOTINCHANNEL(srv->getUserByFd(fd)->getNickname(), params[0])));
		return (-3);
	}
	// Check that user is an operator
	if (findUserOnChannel(itChannel->second->_operators, srv->getUserByFd(fd)) == false
		&& params.size() > 1 && params[1].compare("b") != 0)
	{
		srv->sendClient(fd, numericReply(srv, fd, "482",
										 ERR_CHANOPRIVSNEEDED(params[0])));
		return (-4);
	}
	return (0);
}

void ChannelMode(const int &fd, const std::vector<std::string> &params, Server *srv)
{
	std::string mode = "+";
	std::string modeParams;
	Channel *channel;

	if (checkChannelMode(fd, params, srv) < 0)
		return;
	if (params.size() > 1)
		handleAddRemoveModesChannel(fd, params, srv);
	else
	{
		channel = srv->_channelList.find(params[0])->second;
		if (channel->getKey().empty() == false)
		{
			mode += "k";
			modeParams += channel->getKey();
		}
		if (channel->hasMode(MOD_INVITE) == true)
			mode += "i";
		return (srv->sendClient(fd, numericReply(srv, fd,
			"324", RPL_CHANNELMODEIS(params[0], mode, modeParams))));
	}
}

/* ************************************************************************** */
/* MODE FUNCTION                                                              */
/* ************************************************************************** */

void mode(const int &fd, const std::vector<std::string> &params, const std::string &,
		  Server *srv)
{
	if (params.empty() || params[0].empty())
	{
		return (srv->sendClient(fd, numericReply(srv, fd, "461",
			ERR_NEEDMOREPARAMS(std::string("MODE")))));
	}
	if (isChannel(params[0]) == true)
		ChannelMode(fd, params, srv);
	else
		UserMode(fd, params, srv);
	return;
}
