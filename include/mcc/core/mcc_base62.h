#ifndef INC_BASE62_H
#define INC_BASE62_H
#include "mcc_get.h"

#ifdef ULLONG_MAX
typedef ullong ubase62_t;
#define BASEuPRI "llu"
#define BASEdPRI "lld"
#define BASEoPRI "llo"
#define BASExPRI "llx"
#define BASEXPRI "llX"
#else
typedef ulong ubase62_t;
#define BASEuPRI "llu"
#define BASEdPRI "lld"
#define BASEoPRI "llo"
#define BASExPRI "llx"
#define BASEXPRI "llX"
#endif

int ubase62(
	MCC_GETC *src, ubase62_t *dst, int base,
	bool lowislow, long min_len, long max_len );

#endif
