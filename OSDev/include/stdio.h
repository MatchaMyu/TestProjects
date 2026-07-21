#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>
#include <stdarg.h>

typedef struct FILE FILE;

/* Standard streams */
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

/* File positioning */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);

/* File management */
int remove(const char *pathname);
int rename(const char *oldpath, const char *newpath);

/* Formatted I/O */
int printf(const char *fmt, ...);
int fprintf(FILE *stream, const char *fmt, ...);

int vfprintf(FILE *stream, const char *fmt, va_list args);
int vprintf(const char *fmt, va_list args);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int snprintf(char *str, size_t size, const char *format, ...);

/* Character and string I/O */
int putchar(int c);
int puts(const char *s);

/* Formatted input */
int sscanf(const char *str, const char *format, ...);
int vsscanf(const char *str, const char *format, va_list ap);

/* File I/O */
FILE *fopen(const char *path, const char *mode);
int fclose(FILE *stream);
int fflush(FILE *stream);

size_t fread(
    void *ptr,
    size_t size,
    size_t count,
    FILE *stream
);

size_t fwrite(
    const void *ptr,
    size_t size,
    size_t count,
    FILE *stream
);

#endif
