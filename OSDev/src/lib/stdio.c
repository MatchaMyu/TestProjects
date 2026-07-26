#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

//#include "apps/doomgeneric/doomheaders/resources/doom1_wad.h"
#include "apps/doomgeneric/doomheaders/resources/doom1_wad_dummy.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int vsnprintf(char *str, size_t size, const char *format, va_list args);

struct FILE
{
    const unsigned char *data;
    size_t size;
    size_t pos;
};

static struct FILE stdout_file;
static struct FILE stderr_file;

FILE *stdout = &stdout_file;
FILE *stderr = &stderr_file;

static int is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n' ||
           c == '\r' || c == '\v' || c == '\f';
}

static int hex_value(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

long ftell(FILE *stream)
{
    if (!stream)
        return -1;

    return (long)stream->pos;
}

int remove(const char *pathname)
{
    (void)pathname;
    return -1;
}

int rename(const char *oldpath, const char *newpath)
{
    (void)oldpath;
    (void)newpath;
    return -1;
}

int fflush(FILE *stream)
{
    (void)stream;
    return 0;
}

int vfprintf(FILE *stream, const char *fmt, va_list args)
{
    (void)stream;
    return vprintf(fmt, args);
}

int snprintf(char *str, size_t size, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vsnprintf(str, size, format, args);
    va_end(args);
    return result;
}

FILE *fopen(const char *path, const char *mode)
{
    (void)mode;

    if (strcmp(path, "doom1.wad") == 0 ||
        strcmp(path, "DOOM1.WAD") == 0)
    {
        static FILE wad_file;

        wad_file.data = DOOM1_WAD;
        wad_file.size = DOOM1_WAD_len;
        wad_file.pos = 0;

        return &wad_file;
    }

    return NULL;
}

size_t fread(void *ptr, size_t size, size_t count, FILE *stream)
{
    if (!stream || !ptr || size == 0 || count == 0)
        return 0;

    if (stream->pos >= stream->size)
        return 0;

    if (count > SIZE_MAX / size)
        return 0;

    size_t requested = size * count;
    size_t remaining = stream->size - stream->pos;
    size_t bytes = requested;

    if (bytes > remaining)
        bytes = remaining;

    memcpy(ptr, stream->data + stream->pos, bytes);
    stream->pos += bytes;

    return bytes / size;
}

int fclose(FILE *fp)
{
    (void)fp;
    return 0;
}

int fseek(FILE *stream, long offset, int whence)
{
    if (!stream)
        return -1;

    long base;

    switch (whence)
    {
        case SEEK_SET:
            base = 0;
            break;

        case SEEK_CUR:
            base = (long)stream->pos;
            break;

        case SEEK_END:
            base = (long)stream->size;
            break;

        default:
            return -1;
    }

    long new_pos = base + offset;

    if (new_pos < 0)
        return -1;

    if ((size_t)new_pos > stream->size)
        return -1;

    stream->pos = (size_t)new_pos;
    return 0;
}

static void buf_putc(char *buf, size_t size, size_t *pos, char c)
{
    if (*pos + 1 < size)
        buf[*pos] = c;

    (*pos)++;
}

static void buf_puts(char *buf, size_t size, size_t *pos, const char *s)
{
    if (!s)
        s = "(null)";

    while (*s)
        buf_putc(buf, size, pos, *s++);
}

static void buf_put_uint_width(
    char *buf,
    size_t size,
    size_t *pos,
    uint32_t value,
    int width,
    char padding
)
{
    char tmp[11];
    int digits = 0;

    do
    {
        tmp[digits++] = (char)('0' + (value % 10));
        value /= 10;
    }
    while (value > 0);

    while (digits < width)
    {
        buf_putc(buf, size, pos, padding);
        width--;
    }

    while (digits > 0)
        buf_putc(buf, size, pos, tmp[--digits]);
}

static void buf_put_hex32(char *buf, size_t size, size_t *pos, uint32_t value)
{
    const char *hex = "0123456789abcdef";

    buf_puts(buf, size, pos, "0x");

    for (int i = 28; i >= 0; i -= 4)
        buf_putc(buf, size, pos, hex[(value >> i) & 0xF]);
}

int vsscanf(const char *str, const char *format, va_list ap)
{
    int assigned = 0;

    while (*format) {
        if (is_space(*format)) {
            while (is_space(*str)) str++;
            format++;
            continue;
        }

        if (*format != '%') {
            if (*str != *format)
                break;

            str++;
            format++;
            continue;
        }

        format++;

        if (*format == '%') {
            if (*str != '%')
                break;

            str++;
            format++;
            continue;
        }

        while (is_space(*str)) str++;

        if (*format == 'x') {
            unsigned int value = 0;
            int digits = 0;

            while (hex_value(*str) >= 0) {
                value = value * 16 + hex_value(*str);
                str++;
                digits++;
            }

            if (digits == 0)
                break;

            unsigned int *out = va_arg(ap, unsigned int *);
            *out = value;
            assigned++;
            format++;
            continue;
        }

        // Unsupported format specifier for now
        break;
    }

    return assigned;
}

int sscanf(const char *str, const char *format, ...)
{
    va_list ap;
    int result;

    va_start(ap, format);
    result = vsscanf(str, format, ap);
    va_end(ap);

    return result;
}

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream)
{
    (void)ptr;
    (void)size;
    (void)count;
    (void)stream;

    errno = EIO;
    return 0;
}

int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
    size_t pos = 0;

    while (*fmt)
    {
        if (*fmt != '%')
        {
            buf_putc(buf, size, &pos, *fmt++);
            continue;
        }

        fmt++;

        int width = 0;
        char padding = ' ';

        /* Support %03d */
        if (*fmt == '0')
        {
            padding = '0';
            fmt++;
        }

        /* Support %.3d */
        if (*fmt == '.')
        {
            padding = '0';
            fmt++;
        }

        while (*fmt >= '0' && *fmt <= '9')
        {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }

        if (*fmt == 's')
        {
            char *s = va_arg(args, char *);
            buf_puts(buf, size, &pos, s);
        }
        else if (*fmt == 'c')
        {
            char c = (char)va_arg(args, int);
            buf_putc(buf, size, &pos, c);
        }
        else if (*fmt == 'u')
        {
            uint32_t v = va_arg(args, uint32_t);
            buf_put_uint_width(buf, size, &pos, v, width, padding);
        }
        else if (*fmt == 'i' || *fmt == 'd')
        {
            int v = va_arg(args, int);

            if (v < 0)
            {
                buf_putc(buf, size, &pos, '-');
                v = -v;
            }

            buf_put_uint_width(buf, size, &pos, (uint32_t)v, width, padding);
        }
        else if (*fmt == 'x')
        {
            uint32_t v = va_arg(args, uint32_t);
            buf_put_hex32(buf, size, &pos, v);
        }
        else if (*fmt == 'p')
        {
            uintptr_t v = (uintptr_t)va_arg(args, void *);
            buf_put_hex32(buf, size, &pos, (uint32_t)v);
        }
        else if (*fmt == '%')
        {
            buf_putc(buf, size, &pos, '%');
        }

        fmt++;
    }

    if (size > 0)
    {
        if (pos >= size)
            buf[size - 1] = '\0';
        else
            buf[pos] = '\0';
    }

    return (int)pos;
}
