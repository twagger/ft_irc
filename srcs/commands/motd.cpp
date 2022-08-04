#include "../../includes/commands.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/replies.hpp"
#include "../../includes/exceptions.hpp"

#ifndef MOTD_FILE 
# define MOTD_FILE "conf/motd.txt"
#endif

/**
 * @brief The MOTD command is used to get the "Message Of The Day" of the given
 * server, or current server if <target> is omitted.
 * 
 * Errors handled:
 * - ERR_NOMOTD
 *   
 */

std::string createModtStr(Server *srv, const int &fd, std::string &filename)
{
	std::ifstream   infile;
	std::string	    line;
	std::string	    replyMsg;

    // Try to open file
	infile.exceptions(std::ifstream::failbit);
	try { infile.open(filename.c_str(), std::fstream::in); }
	catch (std::ios_base::failure &e)
	{ throw nomotdException(); }

    // Read the file one line at a time and append the line to the reply
    replyMsg.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("\
        Bienvenue - welcome - bouno vengudo - i bisimila - degemer mad - \
        benvinguts - velkommen"))));
	try {
		while (std::getline(infile, line))
            replyMsg.append(numericReply(srv, fd, "372", RPL_MOTD(line)));
	} catch (std::ios_base::failure &e) {}
    replyMsg.append(numericReply(srv, fd, "372", RPL_MOTD(std::string("Des \
        bisous de la Space team <3"))));

	return (replyMsg);
}

void    motd(const int &fd, const std::vector<std::string> &, \
                       const std::string &,Server *srv)
{
    std::string reply;
    std::string motdFile(MOTD_FILE);

     // COMMAND EXECUTION
    try { 
        reply
        .append(numericReply(srv, fd, "375", RPL_MOTDSTART(srv->getHostname())))
        .append(createModtStr(srv, fd, motdFile))
        .append(numericReply(srv, fd, "376", RPL_ENDOFMOTD));
        srv->sendClient(fd, reply);
    }

    // EXCEPTIONS
    catch (nomotdException &e) {e.reply(srv, fd); return; }
}
