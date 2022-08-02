#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

/* ************************************************************************** */
/* USER		                                                                  */
/* ************************************************************************** */

std::string userModesToStr(User *user)
{
	std::string modes;

	modes.append("+");
	if (user->hasMode(MOD_AWAY))
		modes.append("a");
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
		{
			findPair(modes, i, &pair);
		}
		if (modes[i] == '+' && pair.second > 0)
		{
			addModes(user, modes, i, pair.second + 1);
		}
		else if (modes[i] == '-' && pair.second > 0)
		{
			removeModes(user, modes, i, pair.second + 1);
		}
	}
}

void UserMode(const int &fd, const std::vector<std::string> &params, Server *srv)
{
	User *user = srv->getUserByFd(fd);

	if (params.size() == 1 && params[0] == user->getNickname())
	{ // no nickname given
		return (srv->sendClient(fd, numericReply(srv, fd, "221",
												 RPL_UMODEIS(userModesToStr(user)))));
	}
	else if (srv->getUserByNickname(params[0]) == 0)
	{
		return (srv->sendClient(fd, numericReply(srv, fd, "401",
												 ERR_NOSUCHNICK(params[0]))));
	}
	else if (params[0] != user->getNickname())
	{
		return (srv->sendClient(fd, numericReply(srv, fd, "502",
												 ERR_USERSDONTMATCH)));
	}
	// else if (isNotUserMode(params[1], USERMODES) || isNotChannelMode(params[1], CHANNELMODES))									// replace with pattern matching
	// 	replyMsg = numericReply(srv, fd, "501", ERR_UMODEUNKNOWNFLAG);
	else if (!params[1].empty())
	{
		handleAddRemoveModes(user, params[1]);
	} // we already checked above that param == user's own nickname
}

/* ************************************************************************** */
/* CHANNEL	                                                                  */
/* ************************************************************************** */

void listBannedUser(const int &fdUser, Server *server,
              Channel *channel)
{
    std::string nicknameList;
    std::deque<User *> listBannedUser = channel->_bannedUsers;
    std::deque<User *>::iterator itBannedUser;

    for (itBannedUser = listBannedUser.begin(); itBannedUser != listBannedUser.end();
		itBannedUser++)
        nicknameList += (*itBannedUser)->getNickname() + " ";
    server->sendClient(fdUser, numericReply(server, fdUser,
        "367", RPL_BANLIST(channel->getChannelName(), nicknameList)));
    server->sendClient(fdUser, numericReply(server, fdUser,
		"368", RPL_ENDOFBANLIST(channel->getChannelName())));
}

void addModesChannel(const std::vector<std::string> params, int start, int stop,
					 const int &fd, Server *srv)
{
	Channel *channel = srv->_channelList.find(params[0])->second;
	User *user = NULL;
	
	if (params[2].empty() == false)
		user = srv->getUserByNickname(params[2]);

	for (int i = start; i < stop; i++)
	{
		switch (params[1][i])
		{
		case 'i':
			channel->addMode(MOD_INVITE);
		case 'k':
			if (channel->hasMode(MOD_KEY) == true)
				return (srv->sendClient(fd, numericReply(srv, fd, "467",
					ERR_KEYSET(params[0]))));
			channel->addMode(MOD_KEY);
			channel->setKey(params[2]);
		case 'o':
			if (user == NULL && params[2].empty() == false)
				return (srv->sendClient(fd, numericReply(srv, fd, "441",
					ERR_USERNOTINCHANNEL(params[2], params[0]))));
			else if (params[2].empty() == true)
				return ;
			channel->addMode(MOD_OPERATOR);
			channel->addOperator(user);
		case 'b':
			if (user == NULL && params[2].empty() == false)
				return (srv->sendClient(fd, numericReply(srv, fd, "441",
					ERR_USERNOTINCHANNEL(params[2], params[0]))));
			else if (params[2].empty() == true)
				listBannedUser(fd, srv, channel);
			channel->addMode(MOD_BAN);
			channel->addBannedUser(user);
		default:
			return (srv->sendClient(fd, numericReply(srv, fd, "472",
				ERR_UNKNOWNMODE(params[1], params[0]))));

		}
	}
}

void removeModesChannel(const std::vector<std::string> params, int start, int stop,
						const int &fd, Server *srv)
{
	Channel *channel = srv->_channelList.find(params[0])->second;
	User *user = NULL;
	 
	 if (params[2].empty() == false)
	 	user = srv->getUserByNickname(params[2]);

	for (int i = start; i < stop; i++)
	{
		switch (params[1][i])
		{
		case 'i':
			channel->removeMode(MOD_INVITE);
		case 'k':
			channel->removeMode(MOD_KEY);
		case 'o':
			if (user == NULL && params[2].empty() == false)
				return (srv->sendClient(fd, numericReply(srv, fd, "441",
					ERR_USERNOTINCHANNEL(params[3], params[0]))));
			else if (params[2].empty() == true)
				return ;
			channel->removeMode(MOD_OPERATOR);
		case 'b':
			if (user == NULL && params[2].empty() == false)
				return (srv->sendClient(fd, numericReply(srv, fd, "441",
					ERR_USERNOTINCHANNEL(params[3], params[0]))));
			else if (params[2].empty() == true)
				return ;
			channel->removeMode(MOD_BAN);
		default:
			return (srv->sendClient(fd, numericReply(srv, fd, "472",
				ERR_UNKNOWNMODE(params[1], params[0]))));
		}
	}
	if (params[2].empty() == true)
		return(srv->sendChannel(params[0], clientReply(srv, fd, "MODE " +
			params[0] + " " + params[1])));
	return(srv->sendChannel(params[0], clientReply(srv, fd, "MODE " +
		params[0] + " " + params[1] + " " + params[2])));
}

void handleAddRemoveModesChannel(const int &fd, const std::vector<std::string> params,
								 Server *srv)
{
	std::pair<char, int> pair;

	for (unsigned int i = 0; i < params[1].size(); i++)
	{
		if (params[1][i] == '+' || params[1][i] == '-')
		{
			findPair(params[1], i, &pair);
		}
		if (params[1][i] == '+' && pair.second > 0)
		{
			addModesChannel(params, i, pair.second + 1, fd, srv);
		}
		else if (params[1][i] == '-' && pair.second > 0)
		{
			removeModesChannel(params, i, pair.second + 1, fd, srv);
		}
	}
}

int checkChannelMode(const int &fd, const std::vector<std::string> &params, Server *srv)
{
	std::map<std::string, Channel *>::iterator itChannel;
	std::deque<User *>::iterator itUser;
	std::deque<User *>::iterator itOperator;

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
	itUser = findUserOnChannel(itChannel->second->_users, srv->getUserByFd(fd));
	if (itUser == itChannel->second->_users.end())
	{
		srv->sendClient(fd, numericReply(srv, fd, "441",
										 ERR_USERNOTINCHANNEL(srv->getUserByFd(fd)->getNickname(), params[0])));
		return (-3);
	}
	// Check that user is an operator
	itOperator = findUserOnChannel(itChannel->second->_operators, srv->getUserByFd(fd));
	if (itOperator == itChannel->second->_operators.end())
	{
		srv->sendClient(fd, numericReply(srv, fd, "482",
										 ERR_CHANOPRIVSNEEDED(params[0])));
		return (-4);
	}
	return (0);
}

void ChannelMode(const int &fd, const std::vector<std::string> &params, Server *srv)
{
	if (checkChannelMode(fd, params, srv) < 0)
		return;
	if (params[1].empty() == false)
		handleAddRemoveModesChannel(fd, params, srv);
	else
		return (srv->sendClient(fd, numericReply(srv, fd, "461",
			ERR_NEEDMOREPARAMS(std::string("MODE")))));
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
