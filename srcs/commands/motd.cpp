#include <deque>

#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"

#ifndef MOTD
# define MOTD ""
#endif

// RPL_MOTDSTART(server)
// RPL_MOTD(text)
// RPL_ENDOFMOTD

std::string createModtStr(Server *srv, const int &fd) {
	std::string	replyMsg;
	replyMsg.append(numericReply(srv, fd, "372", RPL_MOTD("Bienvenue - welcome - bouno vengudo - i bisimila - degemer mad - benvinguts - velkommen")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("		           .       .RPL_MOTD(text)                   .       .      .     .      .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("          .    .         .    .            .     ______                    ")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("      .           .             .               ////////")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("                .    .   ________   .  .      /////////     .    .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("           .            |.____.  /\\        ./////////    .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("    .                 .//      \\/  |\\     /////////")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("       .       .    .//          \\ |  \\ /////////       .     .   .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("                    ||.    .    .| |  ///////// .     .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("     .    .         ||           | |//`,/////                .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("             .      \\\\        ./ //  /  \\/   .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("  .                   \\\\.___./ //\\` '   ,_\\     .     .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("          .           .     \\ //////\\ , /   \\                 .    .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("                       .    ///////// \\|  '  |    .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("      .        .          ///////// .   \\ _ /          .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("                        /////////                              .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("                 .   ./////////     .     .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("         .           --------   .                  ..             .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("  .               .        .         .                       .")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("                        ________________________")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("____________------------                        -------------_________")))
	.append(numericReply(srv, fd, "372", RPL_MOTD("Des bisous de la Space team <3")));
	return (replyMsg);
}

const std::string motd(const int &fd, const std::vector<std::string> &, \
                       const std::string &,Server *srv)
{
    size_t      len;
    std::string motd = MOTD;
    std::string reply;
    
    // ignore the params and send MOTD to requester fd
    if (!motd.empty())
    {
        reply.append(numericReply(srv, fd, "375", RPL_MOTDSTART(srv->getHostname())))
        .append(createModtStr(srv, fd))
        .append(numericReply(srv, fd, "376", RPL_ENDOFMOTD));
        return (reply);
    }
    else
        return (numericReply(srv, fd, "422", ERR_NOMOTD));
}
