#include <ctype.h>

int isdigit(int c) { return c >= '0' && c <= '9'; }
int isupper(int c) { return c >= 'A' && c <= 'Z'; }
int islower(int c) { return c >= 'a' && c <= 'z'; }
int isalpha(int c) { return isupper(c) || islower(c); }
int isalnum(int c) { return isalpha(c) || isdigit(c); }

int isspace(int c)
{
    switch (c)
    {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        case '\f':
        case '\v':
            return 1;
        default:
            return 0;
    }
}

int toupper(int c)
{
    if (islower(c)) return c - ('a' - 'A');
    return c;
}

int tolower(int c)
{
    if (isupper(c)) return c + ('a' - 'A');
    return c;
}
