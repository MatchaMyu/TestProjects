#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* dest, int value, size_t n);
int memcmp(const void* a, const void* b, size_t n);
void* memmove(void* dest, const void* src, size_t n);
char *strchr(const char *s, int c);

void itoa(unsigned int n, char* str);
size_t strlen(const char* str);
int strcmp(const char* s1, const char* s2);
int strcasecmp(const char *a, const char *b);
int strncasecmp(const char *a, const char *b, size_t n);
char *strrchr(const char *s, int c);
int strncmp(const char *a, const char *b, size_t n);
char *strdup(const char *s);
char *strncpy(char *dest, const char *src, size_t n);
char *strrchr(const char *s, int c);
char *strstr(const char *haystack, const char *needle);


#endif
