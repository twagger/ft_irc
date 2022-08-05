#include <string>
#include <deque>
#include <cstdlib>
#include <ctime>

std::string botTime(void)
{
    time_t      rawtime = time(NULL);
    struct tm   *timeinfo;
    std::string strTime;

    timeinfo = localtime(&rawtime);
    strTime = std::string(asctime(timeinfo));
    strTime = strTime.substr(0, strTime.length() - 1);

    return (strTime);
}
