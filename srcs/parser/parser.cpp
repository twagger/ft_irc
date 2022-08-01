#include "../../includes/parser.hpp"

#include <iostream>
#include <map>
#include <cstdlib>
#include <climits>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <ios>
#include <cmath>

#define GRAMMAR_FILE "irc.abnf"

/* ************************************************************************** */
/* MAP : SYMBOL AND EXPRESSION OF THE GRAMMAR                                 */
/* ************************************************************************** */
const std::map<const std::string, std::string>	initGrammar(const char *filename)
{

	const std::string 							ws = "\n\r\t\f\v ";
	std::ifstream								infile;
	std::map<const std::string, std::string>	grammar;
	std::string									line;
	std::string									symbol;
	std::string									expression;

	infile.exceptions(std::ifstream::failbit);
	try { infile.open(filename, std::fstream::in); }
	catch (std::ios_base::failure &e)
	{ throw std::runtime_error("grammar file open error"); }

	try {
		while (std::getline(infile, line))
		{
			symbol = line.substr(0, line.find('='))
						 .substr(0, line.find_first_of(ws));
			expression = line.substr(line.find('='))
							 .substr(line.find_first_not_of(ws, 1) + 2);
			grammar[symbol] = expression;
		}

	} catch (std::ios_base::failure &e) {}
	return (grammar);
}

/* ************************************************************************** */
/* UTILITY FUNCTIONS                                                          */
/* ************************************************************************** */
const char  getASCIIChar(const std::string &str)
{
    std::string base("0123456789ABCDEF");

    int result = 0;

    result += base.find(str[0]) * 16;
    result += base.find(str[1]);
    return (static_cast<char>(result));
}

// Miscellaneous
void   setLoop(char	*pattern, int *mandatoryMatch, int *extraMatch, int *len)
{
    char    *end = NULL;

    if (std::isdigit(pattern[0]))
    {
        *mandatoryMatch = static_cast<int>(strtol(pattern, &end, 0));
        if (end[0] == '*')
        {
            if (std::isdigit(end[1]))
                *extraMatch = static_cast<int>(strtol(end + 1, &end, 0));
			else
				++end;
        }
    }
    else
    {
        end = pattern + 1;
        if (std::isdigit(pattern[1]))
            *extraMatch = (int)strtol(end + 1, &end, 0) - *mandatoryMatch;
    }
	*len = end - pattern;
}

/* ************************************************************************** */
/* RECURSIVE SYMBOL MATCH                                                     */
/* ************************************************************************** */
static bool	isMatching(std::string &str, std::string pattern, \
					   std::map<const std::string, std::string> &grammar, int l)
{
	std::string	nextPattern;
	char		next;
	std::string	subString;
	std::string	save;
	bool		result = true;
	int			mandatoryMatch;
	int			extraMatch;
    int         len;
	char		*end = NULL;
    char        first;
    char        last;
	bool		isLoop = false;
	int			i = 0;

	if (str.empty())
		return (true);
	save = str;
	result = false;
	isLoop = false;
	std::cout << "[" << l << "] STR : " << str << " / PATTERN : " << pattern << std::endl;
	while (i < pattern.length())
	{
		next = pattern[i];
		if (std::isspace(next))
			len = 1;
		else if (std::isdigit(next) || next == '*') {
			std::cout << "[" << l << "] LOOP SETUP\n";
			// LOOP SETUP
            mandatoryMatch = 0;
            extraMatch = INT_MAX;
            len = 1;
            setLoop(&pattern[i], &mandatoryMatch, &extraMatch, &len);
			isLoop = true;
		}
		else if (next == '(') {
			std::cout << "[" << l << "] GROUP\n";
			// GROUP
			subString = pattern.substr(i + 2, pattern.find(']', i) - i - 2);
			result = isMatching(str, subString, grammar, l + 1);
			len = subString.length();
		}
		else if (next == '[') {
			std::cout << "[" << l << "] OPTION\n";
			// OPTION
			subString = pattern.substr(i + 2, pattern.find(']', i) - i - 2);
			isMatching(str, subString, grammar, l + 1);
			len += subString.length();
		}
		else if (next == '%') {
			std::cout << "[" << l << "] VALUE-RANGE\n";
			// VALUE OR RANGE
            len = 4;
            first = getASCIIChar(&pattern[i + 2]);
            last = first;
            if (pattern[i + 4] && pattern[i + 4] == '-')
            {
                len = 7;
                last = getASCIIChar(&pattern[i + 5]);
            }
			result = false;
            for (char c = first; c <= last; ++c)
            {
                if (str[0] == c)
                {
					std::cout << "[" << l << "] V-R Match : " << c << std::endl;
                    str.erase(0, 1);
                    result = true;
                    break;
                }
            }
		}
        else if (next == '"') {
			std::cout << "[" << l << "] TERMINAL\n";
            // TERMINAL
			subString = pattern.substr(i + 1, pattern.find('"', i + 1) - i - 1);
			if (str.compare(0, subString.length(), subString) == 0)
			{
				std::cout << "[" << l << "] TERM Match : " << str.substr(0, subString.length()) << std::endl;
            	result = true;
				str.erase(0, subString.length());
			}
			else
				result = false;
			len += subString.length() + 2;
        }
		else if (next == '/') {
			std::cout << "[" << l << "] ALTERNATION\n";
			// ALTERNATION
			if (result != true)
				len = 1;
		}
		else {
			std::cout << "[" << l << "] SYMBOL\n";
			// SYMBOL
			subString = pattern.substr(i, pattern.find(' ') - i);
			result = isMatching(str, grammar[subString], grammar, l + 1);
			len += subString.length();
		}
		if (isLoop && !(std::isdigit(next) || next == '*'))
		{
			std::cout << "[" << l << "] LOOP CONTROL\n";
			// LOOP CONTROL
			// mandatory match
			if (mandatoryMatch > 0)
			{
				if (result != false)
			 		--mandatoryMatch;
			}

			// extra match
			if (mandatoryMatch == 0 && extraMatch > 0)
			{
				isLoop = false;
				i += len;
			}
			else
				len = 0;
		}
		else
			i += len;
	}
    // The result is returned to the upper level
	std::cout << "[" << l << "] Return : " << result << std::endl;
	return (result);
}

/* ************************************************************************** */
/* CHECK A STRING AGAINST A PATTERN                                           */
/* ************************************************************************** */
bool    patternMatch(std::string str, std::string pattern)
{
	std::map<const std::string, std::string>	grammar = initGrammar(GRAMMAR_FILE);
    bool    									result;

	// If pattern is empty, the function stops here
    if (pattern.length() == 0)
        return (str.length() == 0);

	try { result = isMatching(str, pattern, grammar, 0); }
	catch (std::runtime_error &e)
	{ std::cout << e.what() << std::endl; return (false);}
	if (result == false)
		return (false);
    // No error ? that's a match !
	std::cout << "STR " << str << std::endl;
	if (!str.empty())
		return (false);
    return (true);
}

// tester to delete
int main(void)
{
    bool    result;

    std::string str("aaaaab");
    std::string pattern("test");

    result = patternMatch(str, pattern);
    if (result)
        std::cout << "Match !" << std::endl;
    else 
        std::cout << "No match !" << std::endl;
    return (0);
}

