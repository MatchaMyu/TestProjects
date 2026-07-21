#include <unistd.h>
#include "timer.h"

unsigned int sleep(unsigned int seconds)
{
    sleep_ms(seconds * 1000);
    return 0;
}

int usleep(unsigned int usec)
{
    sleep_ms(usec / 1000);
    return 0;
}
