#include "../../includes/exceptions.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/Server.hpp"

/* ************************************************************************** */
/* ERR and RPL EXCEPTIONS FOR COMMANDS                                        */
/* ************************************************************************** */
// Generic functions
void ircException::reply(Server *srv, const int &fd) const throw()
{ srv->sendClient(fd, numericReply(srv, fd, this->code, this->rpl)); }

// Grammar exception
const char	*grammarException::what() const throw()
{ return (msg); }

// 401 - nosuchnickException
nosuchnickException::nosuchnickException(std::string nickname, std::string code)
{ this->code = code; this->rpl = ERR_NOSUCHNICK(nickname); }

// 411 - norecipientException
norecipientException::norecipientException(std::string command, std::string code)
{ this->code = code; this->rpl = ERR_NORECIPIENT(command); }

// 412 - notexttosendException
notexttosendException::notexttosendException(std::string code)
{ this->code = code; this->rpl = ERR_NOTEXTTOSEND; }

// 412 - notexttosendException
toomanytargetsException::toomanytargetsException(std::string target, \
                    std::string errcode, std::string abortmsg, std::string code)
{ this->code = code; this->rpl = ERR_TOOMANYTARGETS(target, errcode, abortmsg);}

// 413 - notoplevelException
notoplevelException::notoplevelException(std::string mask, std::string code)
{ this->code = code; this->rpl = ERR_NOTOPLEVEL(mask); }

// 414 - wildtoplevelException
wildtoplevelException::wildtoplevelException(std::string mask, std::string code)
{ this->code = code; this->rpl = ERR_WILDTOPLEVEL(mask); }

// 404 - cannotsendtochanException
cannotsendtochanException::cannotsendtochanException(std::string channame, \
                                                     std::string code)
{ this->code = code; this->rpl = ERR_CANNOTSENDTOCHAN(channame); }

// 409 - nooriginException
nooriginException::nooriginException(std::string code)
{ this->code = code; this->rpl = ERR_NOORIGIN; }

// 402 - nosuchserverException
nosuchserverException::nosuchserverException(std::string servername, \
                                             std::string code)
{ this->code = code; this->rpl = ERR_NOSUCHSERVER(servername); }

// 481 - noprivilegesException
noprivilegesException::noprivilegesException(std::string code)
{ this->code = code; this->rpl = ERR_NOPRIVILEGES; }

// 461 - needmoreparamsException
needmoreparamsException::needmoreparamsException(std::string command, \
                                                 std::string code)
{ this->code = code; this->rpl = ERR_NEEDMOREPARAMS(command); }

// 483 - cantkillserverException
cantkillserverException::cantkillserverException(std::string code)
{ this->code = code; this->rpl = ERR_CANTKILLSERVER; }

// 422 - nomotdException
nomotdException::nomotdException(std::string code)
{ this->code = code; this->rpl = ERR_NOMOTD; }

// 431 - nonicknamegivenException
nonicknamegivenException::nonicknamegivenException(std::string code)
{ this->code = code; this->rpl = ERR_NONICKNAMEGIVEN; }
