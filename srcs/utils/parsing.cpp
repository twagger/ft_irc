#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"

// Split a string by irc message delimiter (\n\r) and return a string vector
std::vector<std::string>  splitBy(std::string str, const std::string &delimiter, std::string *buffer)
{
    std::vector<std::string>    result;
	size_t                      end;

    // first check 
    end = str.find(delimiter);
    if (end == std::string::npos)
        return (std::vector<std::string>());
    // save first command in vector
    if (end + delimiter.length() > MAX_CMD_LEN)
        throw std::runtime_error("Request too long");
    result.push_back(str.substr(0, end));
    
    // update str
    str.erase(0, end + delimiter.length());

    // loop for other commands
    end = str.find(delimiter);
    while (end != std::string::npos)
    {
        if (end + delimiter.length() > MAX_CMD_LEN)
            throw std::runtime_error("Request too long");
        result.push_back(str.substr(0, end));
        str.erase(0, end + delimiter.length());
        end = str.find(delimiter);
    }
	(*buffer).clear();
	if (!str.empty())
		(*buffer) = str;
    return (result);
}

/**
 * @brief Convert a vector of strings into a vector of Command by splitting 
 *        parameters and command
 * 
 * @param cmd_strings Vector of commands, not splitted by space
 * @return vector of Command where [ PREFIX ], CMD and PARAMS are separated
 */
void  splitCmds(std::vector<std::string> cmd_strings, std::vector<Command> *cmds)
{
    std::vector<std::string>::iterator  mess;
    size_t                              end;
    std::string                         prefix;

    for (mess = cmd_strings.begin(); mess != cmd_strings.end(); ++mess)
    {
        // extract CMD name
        end = mess->find(' ');
        if (end == std::string::npos) // only the command
        {
            if (mess->find(':') == 0) // the only token is a prefix :'(
                throw std::runtime_error("IRC message must have a command");
            cmds->push_back(Command(mess->substr(0, end)));
        }
        else // CMD + params
        {
            // CMD WITH PREFIX
            if (mess->find(':') == 0)
            {
                prefix = mess->substr(1, end);
                mess->erase(0, end + 1);
                end = mess->find(' ');
                if (end == std::string::npos)
                {
                    cmds->push_back(Command(*mess, prefix));
                    mess->clear();
                }
                else
                {
                    cmds->push_back(Command(mess->substr(0, end), \
                                       prefix));
                    mess->erase(0, end + 1);
                }
            }
            else // CMD WITH NO PREFIX
            { 
                cmds->push_back(Command(mess->substr(0, end)));
                end = mess->find(' ');
                if (end == std::string::npos)
                    mess->clear();
                else
                    mess->erase(0, end + 1);
            }
            // PARAMS
            if (!mess->empty())
            {
                while (end != std::string::npos)
                {
                    // if long param starting with ":"
                    if (mess->find(':') == 0)
                    {
                        cmds->back().params.push_back(mess->substr(1, \
                                                           mess->length() - 1));
                        end = std::string::npos;
                    }
                    else
                    {
                        end = mess->find(' ');
                        if (end == std::string::npos)
                            cmds->back().params.push_back(*mess);
                        else
                        {
                            cmds->back().params
                                .push_back(mess->substr(0, end));
                            mess->erase(0, end + 1);
                        }
                    }
                }
            }
        }
    }
}

void	displayCommands(std::vector<Command> cmds) {

	std::vector<Command>::iterator  it;
	std::vector<std::string>::iterator  it2;
	for (it = cmds.begin(); it < cmds.end(); ++it)
	{
		std::cout << "\nPREFIX : " << it->prefix << std::endl;
		std::cout << "CMD : " << it->command << std::endl;
		for (it2 = it->params.begin(); it2 < it->params.end(); ++it2)
		{
			std::cout << "PARAM : " << *it2 << std::endl;
		}
	}
}
