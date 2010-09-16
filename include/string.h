#ifndef __STRING_H
#define __STRING_H

#include "stdint.h"

void *memcpy(void *dest, const void *src, int count);
void *memset(void *dest, uint8_t val, int count);
uint16_t *memsetw(uint16_t *dest, uint16_t val, int count);

int strlen(const char *str);
char *strcpy(char *dest, const char *src);
char *strchr(const char *str, char c);
char *strcat(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);

void putch(const char c);
void puts(const char *str);
void kprintf(char *format,...);

#endif
