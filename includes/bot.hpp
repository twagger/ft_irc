#ifndef BOT_HPP
# define BOT_HPP

# include <string>

// Answers
std::string botQuote(void);
std::string botTime(void);
std::string botPing(const std::string &message);

// Utils
int printError(std::string message, int code, bool with_errno = false);

#endif
