#include <string>
#include <deque>
#include <cstdlib>
#include <fstream>
#include <ctime>

#define FILENAME "conf/botquotes.txt"

std::string botQuote(void)
{
    std::string             quote;
    std::ifstream           infile;
	std::string	            line;
    std::deque<std::string> quotes;
    int                     quoteNum;

    // initialize random seed
    std::srand(time(NULL));

    // Try to open file
	infile.exceptions(std::ifstream::failbit);
	try { infile.open(FILENAME, std::fstream::in); }
	catch (std::ios_base::failure &e)
	{ return ("No quote today."); }

    // Read the file one line at a time and append the line to the quote list
	try {
		while (std::getline(infile, line))
            quotes.push_back(line);
	} catch (std::ios_base::failure &e) {}

    // pick one quote in the list
    quoteNum = std::rand() % quotes.size();
    return (quotes[quoteNum]);
}
