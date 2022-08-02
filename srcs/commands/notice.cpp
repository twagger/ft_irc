#include <vector>

#include "../../includes/utils.hpp"
#include "../../includes/exceptions.hpp"

/* ************************************************************************** */
/* IRC COMMAND : NOTICE                                                       */
/* ************************************************************************** */
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
