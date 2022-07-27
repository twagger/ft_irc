#include "../../includes/commands.hpp"

// simple way to ignore the CAP command sent by clients (it's not in the RFC)
const std::string cap(const int &, const std::vector<std::string> &, \
                      const std::string &, Server *)
{ return (std::string()); }
