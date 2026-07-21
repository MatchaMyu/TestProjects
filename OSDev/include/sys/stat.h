#ifndef SYS_STAT_H
#define SYS_STAT_H

#include <sys/types.h>

struct stat
{
    off_t st_size;
};

int mkdir(const char *path, mode_t mode)
{
    (void)path;
    (void)mode;
    return 0;
}

int stat(const char *path, struct stat *buf);

#endif
