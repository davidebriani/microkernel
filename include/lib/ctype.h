#ifndef LIB_CTYPE_H
#define LIB_CTYPE_H

#define isspace(c) (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#define isprint(c) (c >= ' ' && c <= '~')

#endif
