#include <string>
#include <cstring>

#include "../../includes/parser.hpp"

int match(const char *text, const char *regex);

static bool matchStar(char c, const char *regex, const char *text)
{
    do {
        if (matchMask(text, regex))
            return (true);
    } while (*text != '\0' && (*text++ == c || c == '?'));
    return (false);
}

bool    matchMask(const char *text, const char *regex)
{
    if (regex[0] == '\0')
        return (true);
    if (regex[0] == '\\' && (regex[1] == '*' || regex[1] == '?'))
    {
        if (*text != '\0' && (regex[1] == *text))
            return (matchMask(text + 1, regex + 2));
    }
    if (regex[1] == '*')
        return (matchStar(regex[0], regex + 2, text));
    if (*text != '\0' && (regex[0] == '?' || regex[0] == *text))
        return (matchMask(text + 1, regex + 1));
    return (false);
}
