#include <vector>

#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"

/**
 * @brief The NOTICE command is used similarly to PRIVMSG. The difference
 * between NOTICE and PRIVMSG is that automatic replies MUST NEVER be
 * sent in response to a NOTICE message.
 * 
 * Errors handled (/!\ but not returned to client):
 * - ERR_NORECIPIENT
 * - ERR_NOTEXTTOSEND
 * - ERR_CANNOTSENDTOCHAN
 * - ERR_WILDTOPLEVEL
 * - ERR_TOOMANYTARGETS
 * - ERR_NOSUCHNICKERR_NOMOTD
 *   
 */

void notice(const int &fd, const std::vector<std::string> &params, \
                        const std::string &prefix, Server *srv)
{
    // COMMAND EXECUTION
    try { privmsg(fd, params, prefix, srv); }
    
    // EXCEPTIONS
    catch (grammarException &e) { printError(e.what(), 1, false); return; }
    catch (norecipientException &e) {}
    catch (nosuchnickException &e) {}
    catch (notexttosendException &e) {}
    catch (toomanytargetsException &e) {}
    catch (notoplevelException &e) {}
    catch (wildtoplevelException &e) {}
    catch (cannotsendtochanException &e) {}
}
