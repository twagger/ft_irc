#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/replies.hpp"

std::string createModtStr(Server *srv, const int &fd) {
	std::string	replyMsg;
	replyMsg
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("Bienvenue - welcome - bouno vengudo - i bisimila - degemer mad - benvinguts - velkommen"))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀             ⠀⠀⠀⠀⠀⠀⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣤⣤⣤⣶⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⡿⠛⠉⠙⠛⠛⠛⠛⠻⢿⣿⣷⣤⡀⠀⠀⠀⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⠀⣼⣿⠋⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠈⢻⣿⣿⡄⠀⠀⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⣸⣿⡏⠀⠀⠀⣠⣶⣾⣿⣿⣿⠿⠿⠿⢿⣿⣿⣿⣄⠀⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⣿⣿⠁⠀⠀⢰⣿⣿⣯⠁⠀⠀⠀⠀⠀⠀⠀⠈⠙⢿⣷⡄⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⣀⣤⣴⣶⣶⣿⡟⠀⠀⠀⢸⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣷⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⢰⣿⡟⠋⠉⣹⣿⡇⠀⠀⠀⠘⣿⣿⣿⣿⣷⣦⣤⣤⣤⣶⣶⣶⣶⣿⣿⣿⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⢸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⣸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠉⠻⠿⣿⣿⣿⣿⡿⠿⠿⠛⢻⣿⡇⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⣿⣿⠁⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣧⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⢿⣿⡆⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠸⣿⣧⡀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠃⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠛⢿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⣰⣿⣿⣷⣶⣶⣶⣶⠶⠀⢠⣿⣿⠀⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⣽⣿⡏⠁⠀⠀⢸⣿⡇⠀⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⢹⣿⡆⠀⠀⠀⣸⣿⠇⠀⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⢿⣿⣦⣄⣀⣠⣴⣿⣿⠁⠀⠈⠻⣿⣿⣿⣿⡿⠏⠀⠀⠀⠀ "))))
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⠈⠛⠻⠿⠿⠿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ "))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀             ⠀⠀⠀⠀⠀⠀⠀⠀ "))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("Des bisous de la Space team <3"))));
	return (replyMsg);
}

void    motd(const int &fd, const std::vector<std::string> &, \
                       const std::string &,Server *srv)
{
    std::string reply;
    
    if (!createModtStr(srv, fd).empty())
    {
        reply
        .append(numericReply(srv, fd, "375", RPL_MOTDSTART(srv->getHostname())))
        .append(createModtStr(srv, fd))
        .append(numericReply(srv, fd, "376", RPL_ENDOFMOTD));
        srv->sendClient(fd, reply);
    }
    else
        srv->sendClient(fd, numericReply(srv, fd, "422", ERR_NOMOTD));
}
