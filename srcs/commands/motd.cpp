#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/replies.hpp"

std::string createModtStr(Server *srv, const int &fd) {
	std::string	replyMsg;
	replyMsg
    .append(numericReply(srv, fd, "372", RPL_MOTD(std::string("Bienvenue - welcome - bouno vengudo - i bisimila - degemer mad - benvinguts - velkommen"))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("		           .       .                   .       .      .     .      ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("          .    .         .    .            .     ______                    "))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("      .           .             .               ////////"))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("                .    .   ________   .  .      /////////     .    ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("           .            |.____.  /\\        ./////////    ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("    .                 .//      \\/  |\\     /////////"))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("       .       .    .//          \\ |  \\ /////////       .     .   ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("                    ||.    .    .| |  ///////// .     ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("     .    .         ||           | |//`,/////                ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("             .      \\\\        ./ //  /  \\/   ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("  .                   \\\\.___./ //\\` '   ,_\\     .     ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("          .           .     \\ //////\\ , /   \\                 .    ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("                       .    ///////// \\|  '  |    ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("      .        .          ///////// .   \\ _ /          ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("                        /////////                              ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("                 .   ./////////     .     ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("         .           --------   .                  ..             ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("  .               .        .         .                       ."))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("                        ________________________"))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("____________------------                        -------------_________"))))
	.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("Des bisous de la Space team <3"))));
	return (replyMsg);
}

const std::string motd(const int &fd, const std::vector<std::string> &, \
                       const std::string &,Server *srv)
{
    std::string reply;
    
    if (!createModtStr(srv, fd).empty())
    {
        reply
        .append(numericReply(srv, fd, "375", RPL_MOTDSTART(srv->getHostname())))
        .append(createModtStr(srv, fd))
        .append(numericReply(srv, fd, "376", RPL_ENDOFMOTD));
        return (reply);
    }
    else
        return (numericReply(srv, fd, "422", ERR_NOMOTD));
}
