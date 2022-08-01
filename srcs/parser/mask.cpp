#include <string>

bool    matchMask(std::string str, std::string pattern)
{
    size_t n = str.size();
    size_t m = pattern.size();

    if (m == 0)
        return n == 0;

    bool T[n + 1][m + 1];

    std::memset(&T, false, sizeof(T));

    T[0][0] = true;

    for (size_t j = 1; j <= m; ++j)
    {
        if (pattern[j - 1] == '*')
            T[0][j] = T[0][j - 1];
    }
    for (size_t i = 1; i <= n; ++i)
    {
        for (size_t j = 1; j <= m; ++j)
        {
            if (pattern[j - 1] == str[i - 1] || pattern[j - 1] == '?')
                T[i][j] = T[i - 1][j - 1];
            else if (pattern[j - 1] == '*')
                T[i][j] = T[i][j - 1] || T[i - 1][j];
            else
                T[i][j] = false;
        }
    }
    return T[n][m];
}