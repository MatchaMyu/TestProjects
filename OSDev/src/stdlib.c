#include "stdlib.h"

int abs(int x)
{
    return (x < 0) ? -x : x;
}

double atof(const char *str)
{
    double result = 0.0;
    double fraction = 0.1;
    int sign = 1;

    while (*str == ' ' || *str == '\t')
        str++;

    if (*str == '-') {
        sign = -1;
        str++;
    }
    else if (*str == '+') {
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        result = result * 10.0 + (*str - '0');
        str++;
    }

    if (*str == '.') {
        str++;

        while (*str >= '0' && *str <= '9') {
            result += (*str - '0') * fraction;
            fraction *= 0.1;
            str++;
        }
    }

    return sign * result;
}

int atoi(const char* str)
{
    int result = 0;
    int sign = 1;

    if (*str == '-')
    {
        sign = -1;
        str++;
    }

    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }

    return sign * result;
}

void exit(int status)
{
    (void)status;
    while (1)
        __asm__("hlt");
}

//This can later be implemented into the shell.
int system(const char *command)
{
    (void)command;

    // TODO: Hook into the OS command interpreter.
    return -1;
}
