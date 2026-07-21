#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

//Heap file
void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t size);
void* calloc(size_t count, size_t size);

double atof(const char *str);
int atoi(const char* str);
int abs(int n);

int system(const char *command); //This can be later implemented in the shell.

void exit(int status);
#endif
