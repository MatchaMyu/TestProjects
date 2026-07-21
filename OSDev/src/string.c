#include <stddef.h>
#include <stdint.h>
#include "heap.h"

//helper for strcasecmp
static char lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A');
    return c;
}


//memcpy
void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

void* memmove(void* dest, const void* src, size_t n)
{
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;

    if (d < s)
    {
        // Copy forwards
        for (size_t i = 0; i < n; i++)
            d[i] = s[i];
    }
    else if (d > s)
    {
        // Copy backwards
        for (size_t i = n; i > 0; i--)
            d[i - 1] = s[i - 1];
    }

    return dest;
}

//memset
void* memset(void* dest, int value, size_t n) {
    uint8_t* d = (uint8_t*)dest;

    for (size_t i = 0; i < n; i++) {
        d[i] = (uint8_t)value;
    }

    return dest;
}

int memcmp(const void* a, const void* b, size_t n) {
    const uint8_t* p1 = (const uint8_t*)a;
    const uint8_t* p2 = (const uint8_t*)b;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return (int)p1[i] - (int)p2[i];
        }
    }

    return 0;
}

//itoa
void itoa(unsigned int n, char* str)
{
    int i = 0;

    do {
        str[i++] = '0' + (n % 10);
        n /= 10;
    } while (n);

    str[i] = '\0';

    // reverse
    for (int j = 0; j < i / 2; j++) {
        char t = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = t;
    }
}

size_t strlen(const char* str)
{
    size_t len = 0;

    while (str[len] != '\0')
    {
        len++;
    }

    return len;
}

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }

    return (unsigned char)*a - (unsigned char)*b;
}

int strncmp(const char *a, const char *b, size_t n)
{
    while (n > 0 && *a && (*a == *b))
    {
        a++;
        b++;
        n--;
    }

    if (n == 0)
        return 0;

    return (unsigned char)*a - (unsigned char)*b;
}

int strcasecmp(const char *a, const char *b)
{
    while (*a && lower(*a) == lower(*b))
    {
        a++;
        b++;
    }

    return (unsigned char)lower(*a) - (unsigned char)lower(*b);
}

int strncasecmp(const char *a, const char *b, size_t n)
{
    while (n > 0)
    {
        int ca = (unsigned char)lower(*a);
        int cb = (unsigned char)lower(*b);

        if (ca != cb || *a == '\0' || *b == '\0')
            return ca - cb;

        a++;
        b++;
        n--;
    }

    return 0;
}

char *strchr(const char *s, int c)
{
    unsigned char ch = (unsigned char)c;

    while (*s)
    {
        if ((unsigned char)*s == ch)
            return (char *)s;
        s++;
    }

    return ch == '\0' ? (char *)s : NULL;
}

char *strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

   for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';

   return dest;
}

char *strrchr(const char *s, int c)
{
    const char *last = NULL;
    unsigned char ch = (unsigned char)c;

    do
    {
        if ((unsigned char)*s == ch)
            last = s;
    }
    while (*s++);

    return (char *)last;
}

char *strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);

    if (copy == NULL)
        return NULL;

    memcpy(copy, s, len);

    return copy;
}

char *strstr(const char *haystack, const char *needle)
{
    const char *h;
    const char *n;

    if (*needle == '\0')
        return (char *)haystack;

    while (*haystack)
    {
        h = haystack;
        n = needle;

        while (*h && *n && *h == *n)
        {
            h++;
            n++;
        }

        if (*n == '\0')
            return (char *)haystack;

        haystack++;
    }

    return NULL;
}
