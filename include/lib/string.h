#ifndef LIB_STRING_H
#define LIB_STRING_H

#include <kernel/stdint.h>

void *memcpy(void *dest, const void *src, int32_t count);
void *memset(void *dest, uint8_t val, int32_t count);
uint16_t *memsetw(uint16_t *dest, uint16_t val, int32_t count);
void *memclr(void *dest, int32_t count);
int32_t memcmp(const void *in1, const void *in2, uint32_t count);
uint32_t memidx(const void *in, int8_t value, uint32_t count);

int8_t *strfnd(const int8_t *in, const int8_t *str);
uint32_t strlen(const int8_t *str);
int8_t *strcpy(int8_t *dest, const int8_t *src);
int8_t *strchr(const int8_t *str, int8_t c);
int8_t *strcat(int8_t *dest, const int8_t *src);
int32_t strcmp(const int8_t *s1, const int8_t *s2);
int32_t strdiff(const int8_t *s1, const int8_t *s2);
uint32_t strreadn(const int8_t *in, uint32_t base);
int8_t *strwrt(int8_t *out, const int8_t *in, ...);

#endif
