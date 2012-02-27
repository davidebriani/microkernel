#include "kernel/lib/string.h"
#include "kernel/stdlib.h"

void *memcpy(void *dest, const void *src, int32_t count) {
    const uint8_t *sp = (const uint8_t *)src;
    uint8_t *dp = (uint8_t *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, uint8_t val, int32_t count) {
    uint8_t *temp = (uint8_t *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

uint16_t *memsetw(uint16_t *dest, uint16_t val, int32_t count) {
    uint16_t *temp = (uint16_t *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

int32_t strlen(const int8_t *str) {
    int32_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

int8_t *strchr(const int8_t *str, int8_t c) {
    while (*str) {
	if (*str == c) return (char*)str;
	str++;
    }
    return NULL;
}

int8_t *strcpy(int8_t *dest, const int8_t *src) {
    memcpy(dest, src, strlen(src) + 1);
    return (int8_t *)src;
}

int8_t *strcat(int8_t *dest, const int8_t *src) {
    int8_t *dest2 = dest;
    dest2 += strlen(dest) - 1;
    while (*src) {
	*dest2 = *src;
	src++;
	dest2++;
    }
    *dest2 = 0;
    return dest;
}

int32_t strcmp(const int8_t *s1, const int8_t *s2) {
    while ((*s1) && (*s1 == *s2)) {
	s1++;
	s2++;
    }
    return (*(uint8_t *)s1 - *(uint8_t *)s2);
}
