#include <string>
#include <cstring>

#include "../../includes/parser.hpp"

static bool match(const char *text, const char *regex);
static bool matchStar(char c, const char *regex, const char *text)
{
    do {
        if (match(text, regex))
            return (true);
    } while (*text != '\0' && (*text++ == c || c == '?'));
    return (false);
}

static bool match(const char *text, const char *regex)
{
    if (regex[0] == '\0')
        return (true);
    if (regex[0] == '\\' && (regex[1] == '*' || regex[1] == '?'))
    {
        if (*text != '\0' && (regex[1] == *text))
            return (match(text + 1, regex + 2));
    }
    if (regex[1] == '*')
        return (matchStar(regex[0], regex + 2, text));
    if (*text != '\0' && (regex[0] == '?' || regex[0] == *text))
        return (match(text + 1, regex + 1));
    return (false);
}

// special function to convert * to ?*
bool matchMask(const char *text, const char *regex)
{
    std::string res(regex);
    int         i = 0;

    while (res[i] != '\0')
    {
        if (res[i] == '*')
        {
            res.replace(i, 1, "?*");
            i += 2;
        }
        else
            ++i;
    }
    return (match(text, res.c_str()));
}
