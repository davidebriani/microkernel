#include <lib/string.h>

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

void *memclr(void *dest, int32_t count) {
    memset(dest, 0, count);
    return dest;
}

int32_t memcmp(const void *in1, const void *in2, uint32_t count) {
    const int8_t *ip1 = in1;
    const int8_t *ip2 = in2;
    for (; count; count--)
        if (*ip1++ != *ip2++)
            return 1;
    return 0;
}

uint32_t memidx(const void *in, int8_t value, uint32_t count) {
    const int8_t *ip = in;
    for (; count; count--)
        if (*ip++ == value)
            break;
    return ip - (int8_t *)in;
}

int8_t *strfnd(const int8_t *in, const int8_t *str) {
    int8_t sc, c = *str++;
    uint32_t len = strlen(str);

    if (!c)
        return (int8_t *) in;

    do {
        do {
            sc = *in++;
            if (!sc)
                return (int8_t *) 0;
        } while (sc != c);
    } while (memcmp(in, str, len));

    return (int8_t *)(in - 1);
}

uint32_t strlen(const int8_t *str) {
    uint32_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

int8_t *strchr(const int8_t *str, int8_t c) {
    while (*str) {
	if (*str == c) return (int8_t *) str;
	str++;
    }
    return (int8_t *) 0;
}

int8_t *strcpy(int8_t *dest, const int8_t *src) {
    memcpy(dest, src, strlen(src) + 1);
    return (int8_t *) src;
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

int32_t strdiff(const int8_t *s1, const int8_t *s2) {
    const int8_t *t1 = s1;
    const int8_t *t2 = s2;

    if (!strcmp(s1, s2))
	return 0;

    while ((*s1) && (*s2) && (*s1 == *s2)) {
	s1++;
	s2++;
    }

    if ((*s1) && (*s2))
	return (*(uint8_t *)s1 - *(uint8_t *)s2);
    if (*s1)
	return (s1 - t1);
    else if (*s2)
	return (s2 - t2);
    else
	return -1;
}

uint32_t strreadn(const int8_t *in, uint32_t base) {
    const int8_t *ip = in;
    int32_t num = 0;

    while ((*ip >= '0' && *ip <= '9') || (*ip >= 'a' && *ip <= 'f')) {
        if ((*ip >= '0' && *ip <= '9'))
            num = num * base + *ip++ - '0';

        if ((*ip >= 'a' && *ip <= 'f'))
            num = num * base + *ip++ - 'a' + 10;
    }
    return num;
}

static int8_t *strwrtn(int8_t *out, uint32_t num, uint32_t base) {
    int32_t i;
    int8_t buffer[32];

    if (!num)
        return strcpy(out, "0");

    memclr(buffer, 32);

    for (i = 30; num && i; --i, num /= base)
        buffer[i] = "0123456789abcdef"[num % base];

    return strcpy(out, buffer + i + 1);
}

int8_t *strwrt(int8_t *out, const int8_t *in, ...) {
    int8_t *ret = out;
    int8_t **arg = (int8_t **)&in;
    int8_t c, num[32];
    arg++;

    while ((c = *in++))
    {
        if (c != '%') {
            memcpy(out, &c, 1);
            out += 1;
            continue;
        }

        c = *in++;

        switch (c)
        {
            case 'c':
                memcpy(out, arg, 1);
                out += 1;
                break;
            case 'd':
                strwrtn(num, *(int32_t *)arg, 10);
                strcpy(out, num);
                out += strlen(num);
                break;
            case 's':
                strcpy(out, *(int8_t **)arg);
                out += strlen(*(int8_t **)arg);
                break;
            case 'x':
                strwrtn(num, *(int32_t *)arg, 16);
                strcpy(out, num);
                out += strlen(num);
                break;
        }
        arg++;
    }
    *(out) = '\0';

    return ret;
}
