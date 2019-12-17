#ifndef MCC_STDINT_H
#define MCC_STDINT_H

#include "limits.h"

#ifndef MCC_IS_ROOT_INC
#	ifdef MCC_SYS_WINDOWS_OS
#	include <BaseTsd.h>
#	elif defined( MCC_SYS_LINUX )
#	include <linux/stdint.h>
#	include <linux/inttypes.h>
#	else
#	include <stdint.h>
#	include <inttypes.h>
#	endif
#endif

#ifndef INTPTR_TYPE
#	ifdef __INTPTR_TYPE__
#	define INTPTR_TYPE __INTPTR_TYPE__
#	elif defined( MCC_SYS_LLP64 )
#	define INTPTR_TYPE long long
#	else
#	define INTPTR_TYPE long
#	endif
#endif

#ifndef __intptr_t_defined
#define __intptr_t_defined
typedef signed INTPTR_TYPE intptr_t;
#endif

#ifndef __uintptr_t_defined
#define __uintptr_t_defined
typedef unsigned INTPTR_TYPE uintptr_t;
#endif

#ifndef SIZEOF_INTPTR
#	ifdef __SIZEOF_INTPTR__
#	define SIZEOF_INTPTR __SIZEOF_INTPTR__
#	elif defined( MCC_SYS_LLP64 )
#	define SIZEOF_INTPTR SIZEOF_LLONG
#	else
#	define SIZEOF_INTPTR SIZEOF_LONG
#	endif
#endif

#ifndef INTPTR_WIDTH
#	ifdef __INTPTR_WIDTH__
#	define INTPTR_WIDTH __INTPTR_WIDTH__
#	elif defined( MCC_SYS_LLP64 )
#	define INTPTR_WIDTH LLONG_WIDTH
#	else
#	define INTPTR_WIDTH LONG_WIDTH
#	endif
#endif

#ifndef UINTPTR_C
#	ifdef __UINTPTR_C
#	define UINTPTR_C __UINTPTR_C
#	elif defined( MCC_SYS_LLP64 )
#	define UINTPTR_C( VAL ) VAL##ull
#	else
#	define UINTPTR_C( VAL ) VAL##ul
#	endif
#endif

#ifndef INTPTR_C
#	ifdef __INTPTR_C
#	define INTPTR_C __INTPTR_C
#	elif defined( MCC_SYS_LLP64 )
#	define INTPTR_C( VAL ) VAL##ll
#	else
#	define INTPTR_C( VAL ) VAL##l
#	endif
#endif

#ifndef UINTPTR_MAX
#	ifdef __UINTPTR_MAX__
#	define UINTPTR_MAX __UINTPTR_MAX__
#	else
#	define UINTPTR_MAX (~UINTPTR_C(0))
#	endif
#endif

#ifndef INTPTR_MAX
#	ifdef __INTPTR_MAX__
#	define INTPTR_MAX __INTPTR_MAX__
#	else
#	define INTPTR_MAX (INTPTR_C(0) | (UINTPTR_MAX >> 1))
#	endif
#endif

#ifndef INTPTR_MIN
#	ifdef __INTPTR_MIN__
#	define INTPTR_MIN __INTPTR_MIN__
#	else
#	define INTPTR_MIN (INTPTR_C(-1) ^ INTPTR_MAX)
#	endif
#endif

#ifndef PRI_INTPTR
#	ifdef __PRI_INTPTR__
#	define PRI_INTPTR __PRI_INTPTR__
#	elif defined( MCC_SYS_LLP64 )
#	define PRI_INTPTR "ll"
#	else
#	define PRI_INTPTR "l"
#	endif
#endif

#ifndef SCN_INTPTR
#	ifdef __SCN_INTPTR__
#	define SCN_INTPTR __SCN_INTPTR__
#	elif defined( MCC_SYS_LLP64 )
#	define SCN_INTPTR "ll"
#	else
#	define SCN_INTPTR "l"
#	endif
#endif

#ifndef PRIXINTPTR
#define PRIXINTPTR PRI_INTPTR "X"
#endif
#ifndef PRIdINTPTR
#define PRIdINTPTR PRI_INTPTR "d"
#endif
#ifndef PRIiINTPTR
#define PRIiINTPTR PRI_INTPTR "d"
#endif
#ifndef PRIoINTPTR
#define PRIoINTPTR PRI_INTPTR "o"
#endif
#ifndef PRIuINTPTR
#define PRIuINTPTR PRI_INTPTR "u"
#endif
#ifndef PRIxINTPTR
#define PRIxINTPTR PRI_INTPTR "x"
#endif
#ifndef SCNXINTPTR
#define SCNXINTPTR PRI_INTPTR "X"
#endif
#ifndef SCNdINTPTR
#define SCNdINTPTR SCN_INTPTR "d"
#endif
#ifndef SCNiINTPTR
#define SCNiINTPTR SCN_INTPTR "d"
#endif
#ifndef SCNoINTPTR
#define SCNoINTPTR SCN_INTPTR "o"
#endif
#ifndef SCNuINTPTR
#define SCNuINTPTR SCN_INTPTR "u"
#endif
#ifndef SCNxINTPTR
#define SCNxINTPTR SCN_INTPTR "x"
#endif

#ifndef INTMAX_TYPE
#	ifdef __INTMAX_TYPE__
#	define INTMAX_TYPE __INTMAX_TYPE__
#	elif defined( MCC_HAVE_LLONG ) && LLONG_MAX > LONG_MAX
#	define INTMAX_TYPE long long
#	else
#	define INTMAX_TYPE long
#	endif
#endif

#ifndef __intmax_t_defined
#define __intmax_t_defined
typedef signed INTMAX_TYPE intmax_t;
#endif

#ifndef __uintmax_t_defined
#define __uintmax_t_defined
typedef unsigned INTMAX_TYPE uintmax_t;
#endif

#ifndef SIZEOF_INTMAX
#	ifdef __SIZEOF_INTMAX__
#	define SIZEOF_INTMAX __SIZEOF_INTMAX__
#	elif defined( MCC_HAVE_LLONG ) && LLONG_MAX > LONG_MAX
#	define SIZEOF_INTMAX SIZEOF_LLONG
#	else
#	define SIZEOF_INTMAX SIZEOF_LONG
#	endif
#endif

#ifndef INTMAX_WIDTH
#	ifdef __INTMAX_WIDTH__
#	define INTMAX_WIDTH __INTMAX_WIDTH__
#	elif defined( MCC_HAVE_LLONG ) && LLONG_MAX > LONG_MAX
#	define INTMAX_WIDTH LLONG_WIDTH
#	else
#	define INTMAX_WIDTH LONG_WIDTH
#	endif
#endif

#ifndef UINTMAX_C
#	ifdef __UINTMAX_C
#	define UINTMAX_C __UINTMAX_C
#	elif defined( MCC_HAVE_LLONG ) && LLONG_MAX > LONG_MAX
#	define UINTMAX_C( VAL ) VAL##ull
#	else
#	define UINTMAX_C( VAL ) VAL##ul
#	endif
#endif

#ifndef INTMAX_C
#	ifdef __INTMAX_C
#	define INTMAX_C __INTMAX_C
#	elif defined( MCC_HAVE_LLONG ) && LLONG_MAX > LONG_MAX
#	define INTMAX_C( VAL ) VAL##ll
#	else
#	define INTMAX_C( VAL ) VAL##l
#	endif
#endif

#ifndef UINTMAX_MAX
#	ifdef __UINTMAX_MAX__
#	define UINTMAX_MAX __UINTMAX_MAX__
#	else
#	define UINTMAX_MAX (~UINTMAX_C(0))
#	endif
#endif

#ifndef INTMAX_MAX
#	ifdef __INTMAX_MAX__
#	define INTMAX_MAX __INTMAX_MAX__
#	else
#	define INTMAX_MAX (INTMAX_C(0) | (UINTMAX_MAX >> 1))
#	endif
#endif

#ifndef INTMAX_MIN
#	ifdef __INTMAX_MIN__
#	define INTMAX_MIN __INTMAX_MIN__
#	else
#	define INTMAX_MIN (INTMAX_C(-1) ^ INTMAX_MAX)
#	endif
#endif

#ifndef PRI_INTMAX
#	ifdef __PRI_INTMAX__
#	define PRI_INTMAX __PRI_INTMAX__
#	elif defined( MCC_HAVE_LLONG ) && LLONG_MAX > LONG_MAX
#	define PRI_INTMAX "ll"
#	else
#	define PRI_INTMAX "l"
#	endif
#endif

#ifndef SCN_INTMAX
#	ifdef __SCN_INTMAX__
#	define SCN_INTMAX __SCN_INTMAX__
#	elif defined( MCC_HAVE_LLONG ) && LLONG_MAX > LONG_MAX
#	define SCN_INTMAX "ll"
#	else
#	define SCN_INTMAX "l"
#	endif
#endif

#ifndef PRIXINTMAX
#define PRIXINTMAX PRI_INTMAX "X"
#endif
#ifndef PRIdINTMAX
#define PRIdINTMAX PRI_INTMAX "d"
#endif
#ifndef PRIiINTMAX
#define PRIiINTMAX PRI_INTMAX "d"
#endif
#ifndef PRIoINTMAX
#define PRIoINTMAX PRI_INTMAX "o"
#endif
#ifndef PRIuINTMAX
#define PRIuINTMAX PRI_INTMAX "u"
#endif
#ifndef PRIxINTMAX
#define PRIxINTMAX PRI_INTMAX "x"
#endif
#ifndef SCNXINTMAX
#define SCNXINTMAX PRI_INTMAX "X"
#endif
#ifndef SCNdINTMAX
#define SCNdINTMAX SCN_INTMAX "d"
#endif
#ifndef SCNiINTMAX
#define SCNiINTMAX SCN_INTMAX "d"
#endif
#ifndef SCNoINTMAX
#define SCNoINTMAX SCN_INTMAX "o"
#endif
#ifndef SCNuINTMAX
#define SCNuINTMAX SCN_INTMAX "u"
#endif
#ifndef SCNxINTMAX
#define SCNxINTMAX SCN_INTMAX "x"
#endif

#ifndef __imaxdiv_t_defined
#define __imaxdiv_t_defined
typedef struct imaxdiv {
	intmax_t quo;
	intmax_t rem;
} imaxdiv_t;
#endif

#ifndef INT_LEAST8_WIDTH
#ifdef __INT_LEAST8_WIDTH__
#define INT_LEAST8_WIDTH __INT_LEAST8_WIDTH__
#else
#define INT_LEAST8_WIDTH INT_WIDTH
#endif
#endif

#ifndef INT_LEAST8_TYPE
#	ifdef __INT_LEAST8_TYPE__
#	define INT_LEAST8_TYPE __INT_LEAST8_TYPE__
#	elif defined( INT_LEAST8_WIDTH )
#		if INT_LEAST8_WIDTH == INT_WIDTH
#		define INT_LEAST8_TYPE int
#		elif INT_LEAST8_WIDTH == SHRT_WIDTH
#		define INT_LEAST8_TYPE short
#		else
#		define INT_LEAST8_TYPE char
#		endif
#	else
#	define INT_LEAST8_TYPE int
#	define INT_LEAST8_WIDTH INT_WIDTH
#	define PRI_LEAST8
#	define SCN_LEAST8
#	endif
#endif

#ifndef UINT_LEAST8_C
#	ifdef __UINT_LEAST8_C
#	define UINT_LEAST8_C __UINT_LEAST8_C
#	else
#	define UINT_LEAST8_C( VAL ) VAL##u
#	endif
#endif

#ifndef INT_LEAST8_C
#	ifdef __INT_LEAST8_C
#	define INT_LEAST8_C __INT_LEAST8_C
#	else
#	define INT_LEAST8_C( VAL ) VAL
#	endif
#endif

#ifndef UINT_LEAST8_MAX
#	ifdef __UINT_LEAST8_MAX__
#	define UINT_LEAST8_MAX __UINT_LEAST8_MAX__
#	else
#	define UINT_LEAST8_MAX (~UINT_LEAST8_C(0))
#	endif
#endif

#ifndef INT_LEAST8_MAX
#	ifdef __INT_LEAST8_MAX__
#	define INT_LEAST8_MAX __INT_LEAST8_MAX__
#	else
#	define INT_LEAST8_MAX (INT_LEAST8_C(0) | (UINT_LEAST8_MAX >> 1))
#	endif
#endif

#ifndef INT_LEAST8_MIN
#	ifdef __INT_LEAST8_MIN__
#	define INT_LEAST8_MIN __INT_LEAST8_MIN__
#	else
#	define INT_LEAST8_MIN (INT_LEAST8_C(-1) ^ INT_LEAST8_MAX)
#	endif
#endif

#ifndef PRI_LEAST8
#	if INT_LEAST8_WIDTH == INT_WIDTH
#	define PRI_LEAST8
#	elif INT_LEAST8_WIDTH == SHRT_WIDTH
#	define PRI_LEAST8 "h"
#	else
#	define PRI_LEAST8 "hh"
#	endif
#endif

#ifndef SCN_LEAST8
#	if INT_LEAST8_WIDTH == INT_WIDTH
#	define SCN_LEAST8
#	elif INT_LEAST8_WIDTH == SHRT_WIDTH
#	define SCN_LEAST8 "h"
#	else
#	define SCN_LEAST8 "hh"
#	endif
#endif

#ifndef PRIXLEAST8
#define PRIXLEAST8 PRI_LEAST8 "X"
#endif
#ifndef PRIdLEAST8
#define PRIdLEAST8 PRI_LEAST8 "d"
#endif
#ifndef PRIiLEAST8
#define PRIiLEAST8 PRI_LEAST8 "d"
#endif
#ifndef PRIoLEAST8
#define PRIoLEAST8 PRI_LEAST8 "o"
#endif
#ifndef PRIuLEAST8
#define PRIuLEAST8 PRI_LEAST8 "u"
#endif
#ifndef PRIxLEAST8
#define PRIxLEAST8 PRI_LEAST8 "x"
#endif
#ifndef SCNXLEAST8
#define SCNXLEAST8 PRI_LEAST8 "X"
#endif
#ifndef SCNdLEAST8
#define SCNdLEAST8 SCN_LEAST8 "d"
#endif
#ifndef SCNiLEAST8
#define SCNiLEAST8 SCN_LEAST8 "d"
#endif
#ifndef SCNoLEAST8
#define SCNoLEAST8 SCN_LEAST8 "o"
#endif
#ifndef SCNuLEAST8
#define SCNuLEAST8 SCN_LEAST8 "u"
#endif
#ifndef SCNxLEAST8
#define SCNxLEAST8 SCN_LEAST8 "x"
#endif

#ifndef __int_least8_t_defined
#define __int_least8_t_defined
typedef signed INT_LEAST8_TYPE int_least8_t;
#endif
#ifndef __uint_least8_t_defined
#define __uint_least8_t_defined
typedef unsigned INT_LEAST8_TYPE uint_least8_t;
#endif

#if MCC_CPU_WORD_WIDTH >= 16
#ifndef INT_LEAST16_WIDTH
#ifdef __INT_LEAST16_WIDTH__
#define INT_LEAST16_WIDTH __INT_LEAST16_WIDTH__
#else
#define INT_LEAST16_WIDTH INT_WIDTH
#endif
#endif

#ifndef INT_LEAST16_TYPE
#	ifdef __INT_LEAST16_TYPE__
#	define INT_LEAST16_TYPE __INT_LEAST16_TYPE__
#	elif defined( INT_LEAST16_WIDTH )
#		if INT_LEAST16_WIDTH == INT_WIDTH
#		define INT_LEAST16_TYPE int
#		elif INT_LEAST16_WIDTH == SHRT_WIDTH
#		define INT_LEAST16_TYPE short
#		else
#		define INT_LEAST16_TYPE char
#		endif
#	else
#	define INT_LEAST16_TYPE int
#	define INT_LEAST16_WIDTH INT_WIDTH
#	define PRI_LEAST16
#	define SCN_LEAST16
#	endif
#endif

#ifndef UINT_LEAST16_C
#	ifdef __UINT_LEAST16_C
#	define UINT_LEAST16_C __UINT_LEAST16_C
#	else
#	define UINT_LEAST16_C( VAL ) VAL##u
#	endif
#endif

#ifndef INT_LEAST16_C
#	ifdef __INT_LEAST16_C
#	define INT_LEAST16_C __INT_LEAST16_C
#	else
#	define INT_LEAST16_C( VAL ) VAL
#	endif
#endif

#ifndef UINT_LEAST16_MAX
#	ifdef __UINT_LEAST16_MAX__
#	define UINT_LEAST16_MAX __UINT_LEAST16_MAX__
#	else
#	define UINT_LEAST16_MAX (~UINT_LEAST16_C(0))
#	endif
#endif

#ifndef INT_LEAST16_MAX
#	ifdef __INT_LEAST16_MAX__
#	define INT_LEAST16_MAX __INT_LEAST16_MAX__
#	else
#	define INT_LEAST16_MAX (INT_LEAST16_C(0) | (UINT_LEAST16_MAX >> 1))
#	endif
#endif

#ifndef INT_LEAST16_MIN
#	ifdef __INT_LEAST16_MIN__
#	define INT_LEAST16_MIN __INT_LEAST16_MIN__
#	else
#	define INT_LEAST16_MIN (INT_LEAST16_C(-1) ^ INT_LEAST16_MAX)
#	endif
#endif

#ifndef PRI_LEAST16
#	if INT_LEAST16_WIDTH == INT_WIDTH
#	define PRI_LEAST16
#	elif INT_LEAST16_WIDTH == SHRT_WIDTH
#	define PRI_LEAST16 "h"
#	else
#	define PRI_LEAST16 "hh"
#	endif
#endif

#ifndef SCN_LEAST16
#	if INT_LEAST16_WIDTH == INT_WIDTH
#	define SCN_LEAST16
#	elif INT_LEAST16_WIDTH == SHRT_WIDTH
#	define SCN_LEAST16 "h"
#	else
#	define SCN_LEAST16 "hh"
#	endif
#endif

#ifndef PRIXLEAST16
#define PRIXLEAST16 PRI_LEAST16 "X"
#endif
#ifndef PRIdLEAST16
#define PRIdLEAST16 PRI_LEAST16 "d"
#endif
#ifndef PRIiLEAST16
#define PRIiLEAST16 PRI_LEAST16 "d"
#endif
#ifndef PRIoLEAST16
#define PRIoLEAST16 PRI_LEAST16 "o"
#endif
#ifndef PRIuLEAST16
#define PRIuLEAST16 PRI_LEAST16 "u"
#endif
#ifndef PRIxLEAST16
#define PRIxLEAST16 PRI_LEAST16 "x"
#endif
#ifndef SCNXLEAST16
#define SCNXLEAST16 PRI_LEAST16 "X"
#endif
#ifndef SCNdLEAST16
#define SCNdLEAST16 SCN_LEAST16 "d"
#endif
#ifndef SCNiLEAST16
#define SCNiLEAST16 SCN_LEAST16 "d"
#endif
#ifndef SCNoLEAST16
#define SCNoLEAST16 SCN_LEAST16 "o"
#endif
#ifndef SCNuLEAST16
#define SCNuLEAST16 SCN_LEAST16 "u"
#endif
#ifndef SCNxLEAST16
#define SCNxLEAST16 SCN_LEAST16 "x"
#endif

#ifndef __int_least16_t_defined
#define __int_least16_t_defined
typedef signed INT_LEAST16_TYPE int_least16_t;
#endif
#ifndef __uint_least16_t_defined
#define __uint_least16_t_defined
typedef unsigned INT_LEAST16_TYPE uint_least16_t;
#endif

#if MCC_CPU_WORD_WIDTH >= 32
#ifndef INT_LEAST32_WIDTH
#ifdef __INT_LEAST32_WIDTH__
#define INT_LEAST32_WIDTH __INT_LEAST32_WIDTH__
#elif (~0u) >> 31
#define INT_LEAST32_WIDTH INT_WIDTH
#else
#define INT_LEAST32_WIDTH LONG_WIDTH
#endif
#endif

#ifndef INT_LEAST32_TYPE
#	ifdef __INT_LEAST32_TYPE__
#	define INT_LEAST32_TYPE __INT_LEAST32_TYPE__
#	elif defined( INT_LEAST32_WIDTH )
#		if INT_LEAST32_WIDTH == INT_WIDTH
#		define INT_LEAST32_TYPE int
#		else
#		define INT_LEAST32_TYPE long
#		endif
#	elif INT_WIDTH >= 32
#	define INT_LEAST32_TYPE int
#	define INT_LEAST32_WIDTH INT_WIDTH
#	define PRI_LEAST32
#	define SCN_LEAST32
#	else
#	define INT_LEAST32_TYPE long
#	define INT_LEAST32_WIDTH LONG_WIDTH
#	define PRI_LEAST32 "l"
#	define SCN_LEAST32 "l"
#	endif
#endif

#ifndef UINT_LEAST32_C
#	ifdef __UINT_LEAST32_C
#	define UINT_LEAST32_C __UINT_LEAST32_C
#	elif (~0u) >> 31
#	define UINT_LEAST32_C( VAL ) VAL##u
#	else
#	define UINT_LEAST32_C( VAL ) VAL##ul
#	endif
#endif

#ifndef INT_LEAST32_C
#	ifdef __INT_LEAST32_C
#	define INT_LEAST32_C __INT_LEAST32_C
#	elif (~0u) >> 31
#	define INT_LEAST32_C( VAL ) VAL
#	else
#	define INT_LEAST32_C( VAL ) VAL##l
#	endif
#endif

#ifndef UINT_LEAST32_MAX
#	ifdef __UINT_LEAST32_MAX__
#	define UINT_LEAST32_MAX __UINT_LEAST32_MAX__
#	else
#	define UINT_LEAST32_MAX (~UINT_LEAST32_C(0))
#	endif
#endif

#ifndef INT_LEAST32_MAX
#	ifdef __INT_LEAST32_MAX__
#	define INT_LEAST32_MAX __INT_LEAST32_MAX__
#	else
#	define INT_LEAST32_MAX (INT_LEAST32_C(0) | (UINT_LEAST32_MAX >> 1))
#	endif
#endif

#ifndef INT_LEAST32_MIN
#	ifdef __INT_LEAST32_MIN__
#	define INT_LEAST32_MIN __INT_LEAST32_MIN__
#	else
#	define INT_LEAST32_MIN (INT_LEAST32_C(-1) ^ INT_LEAST32_MAX)
#	endif
#endif

#ifndef PRI_LEAST32
#	if INT_LEAST32_WIDTH == INT_WIDTH
#	define PRI_LEAST32
#	else
#	define PRI_LEAST32 "l"
#	endif
#endif

#ifndef SCN_LEAST32
#	if INT_LEAST32_WIDTH == INT_WIDTH
#	define SCN_LEAST32
#	else
#	define SCN_LEAST32 "l"
#	endif
#endif

#ifndef PRIXLEAST32
#define PRIXLEAST32 PRI_LEAST32 "X"
#endif
#ifndef PRIdLEAST32
#define PRIdLEAST32 PRI_LEAST32 "d"
#endif
#ifndef PRIiLEAST32
#define PRIiLEAST32 PRI_LEAST32 "d"
#endif
#ifndef PRIoLEAST32
#define PRIoLEAST32 PRI_LEAST32 "o"
#endif
#ifndef PRIuLEAST32
#define PRIuLEAST32 PRI_LEAST32 "u"
#endif
#ifndef PRIxLEAST32
#define PRIxLEAST32 PRI_LEAST32 "x"
#endif
#ifndef SCNXLEAST32
#define SCNXLEAST32 PRI_LEAST32 "X"
#endif
#ifndef SCNdLEAST32
#define SCNdLEAST32 SCN_LEAST32 "d"
#endif
#ifndef SCNiLEAST32
#define SCNiLEAST32 SCN_LEAST32 "d"
#endif
#ifndef SCNoLEAST32
#define SCNoLEAST32 SCN_LEAST32 "o"
#endif
#ifndef SCNuLEAST32
#define SCNuLEAST32 SCN_LEAST32 "u"
#endif
#ifndef SCNxLEAST32
#define SCNxLEAST32 SCN_LEAST32 "x"
#endif

#ifndef __int_least32_t_defined
#define __int_least32_t_defined
typedef signed INT_LEAST32_TYPE int_least32_t;
#endif
#ifndef __uint_least32_t_defined
#define __uint_least32_t_defined
typedef unsigned INT_LEAST32_TYPE uint_least32_t;
#endif

#if MCC_CPU_WORD_WIDTH >= 64
#ifndef INT_LEAST64_WIDTH
#	ifdef __INT_LEAST64_WIDTH__
#	define INT_LEAST64_WIDTH __INT_LEAST32_WIDTH__
#	elif (~0u) >> 63
#	define INT_LEAST32_WIDTH INT_WIDTH
#	elif (~0ul) >> 63
#	define INT_LEAST32_WIDTH LONG_WIDTH
#	else
#	define INT_LEAST64_WIDTH LLONG_WIDTH
#	endif
#endif

#ifndef INT_LEAST64_TYPE
#	ifdef __INT_LEAST64_TYPE__
#	define INT_LEAST64_TYPE __INT_LEAST64_TYPE__
#	elif defined( INT_LEAST64_WIDTH )
#		if INT_LEAST64_WIDTH == INT_WIDTH
#		define INT_LEAST64_TYPE int
#		elif INT_LEAST64_WIDTH == LONG_WIDTH
#		define INT_LEAST64_TYPE long
#		else
#		define INT_LEAST64_TYPE long long
#		endif
#	else
#	define INT_LEAST64_TYPE long long
#	define INT_LEAST64_WIDTH LLONG_WIDTH
#	define PRI_LEAST64 "ll"
#	define SCN_LEAST64 "ll"
#	endif
#endif

#ifndef UINT_LEAST64_C
#	ifdef __UINT_LEAST64_C
#	define UINT_LEAST64_C __UINT_LEAST64_C
#	elif (~0u) >> 63
#	define UINT_LEAST64_C( VAL ) VAL##u
#	elif (~0ul) >> 63
#	define UINT_LEAST64_C( VAL ) VAL##ul
#	else
#	define UINT_LEAST64_C( VAL ) VAL##ull
#	endif
#endif

#ifndef INT_LEAST64_C
#	ifdef __INT_LEAST64_C
#	define INT_LEAST64_C __INT_LEAST64_C
#	elif (~0u) >> 63
#	define INT_LEAST64_C( VAL ) VAL
#	elif (~0ul) >> 63
#	define INT_LEAST64_C( VAL ) VAL##l
#	else
#	define INT_LEAST64_C( VAL ) VAL##ll
#	endif
#endif

#ifndef UINT_LEAST64_MAX
#	ifdef __UINT_LEAST64_MAX__
#	define UINT_LEAST64_MAX __UINT_LEAST64_MAX__
#	else
#	define UINT_LEAST64_MAX (~UINT_LEAST64_C(0))
#	endif
#endif

#ifndef INT_LEAST64_MAX
#	ifdef __INT_LEAST64_MAX__
#	define INT_LEAST64_MAX __INT_LEAST64_MAX__
#	else
#	define INT_LEAST64_MAX (INT_LEAST64_C(0) | (UINT_LEAST64_MAX >> 1))
#	endif
#endif

#ifndef INT_LEAST64_MIN
#	ifdef __INT_LEAST64_MIN__
#	define INT_LEAST64_MIN __INT_LEAST64_MIN__
#	else
#	define INT_LEAST64_MIN (INT_LEAST64_C(-1) ^ INT_LEAST64_MAX)
#	endif
#endif

#ifndef PRI_LEAST64
#	if INT_LEAST64_WIDTH == INT_WIDTH
#	define PRI_LEAST64
#	elif INT_LEAST64_WIDTH == LONG_WIDTH
#	define PRI_LEAST64 "l"
#	else
#	define PRI_LEAST64 "ll"
#	endif
#endif

#ifndef SCN_LEAST64
#	if INT_LEAST64_WIDTH == INT_WIDTH
#	define SCN_LEAST64
#	elif INT_LEAST64_WIDTH == LONG_WIDTH
#	define SCN_LEAST64 "l"
#	else
#	define SCN_LEAST64 "ll"
#	endif
#endif

#ifndef PRIXLEAST64
#define PRIXLEAST64 PRI_LEAST64 "X"
#endif
#ifndef PRIdLEAST64
#define PRIdLEAST64 PRI_LEAST64 "d"
#endif
#ifndef PRIiLEAST64
#define PRIiLEAST64 PRI_LEAST64 "d"
#endif
#ifndef PRIoLEAST64
#define PRIoLEAST64 PRI_LEAST64 "o"
#endif
#ifndef PRIuLEAST64
#define PRIuLEAST64 PRI_LEAST64 "u"
#endif
#ifndef PRIxLEAST64
#define PRIxLEAST64 PRI_LEAST64 "x"
#endif
#ifndef SCNXLEAST64
#define SCNXLEAST64 PRI_LEAST64 "X"
#endif
#ifndef SCNdLEAST64
#define SCNdLEAST64 SCN_LEAST64 "d"
#endif
#ifndef SCNiLEAST64
#define SCNiLEAST64 SCN_LEAST64 "d"
#endif
#ifndef SCNoLEAST64
#define SCNoLEAST64 SCN_LEAST64 "o"
#endif
#ifndef SCNuLEAST64
#define SCNuLEAST64 SCN_LEAST64 "u"
#endif
#ifndef SCNxLEAST64
#define SCNxLEAST64 SCN_LEAST64 "x"
#endif

#ifndef __int_least64_t_defined
#define __int_least64_t_defined
typedef signed INT_LEAST64_TYPE int_least64_t;
#endif
#ifndef __uint_least64_t_defined
#define __uint_least64_t_defined
typedef unsigned INT_LEAST64_TYPE uint_least64_t;
#endif
#endif
#endif
#endif

#ifndef INT_FAST8_WIDTH
#ifdef __INT_FAST8_WIDTH__
#define INT_FAST8_WIDTH
#endif
#endif

#ifndef INT_FAST8_TYPE
#	ifdef __INT_FAST8_TYPE__
#	define INT_FAST8_TYPE __INT_FAST8_TYPE__
#	elif defined( INT_FAST8_WIDTH )
#		if INT_FAST8_WIDTH == INT_WIDTH
#		define INT_FAST8_TYPE int
#		elif INT_FAST8_WIDTH == SHRT_WIDTH
#		define INT_FAST8_TYPE short
#		else
#		define INT_FAST8_TYPE char
#		endif
#	else
#	define INT_FAST8_TYPE int
#	define INT_FAST8_WIDTH INT_WIDTH
#	define PRI_FAST8
#	define SCN_FAST8
#	endif
#endif

#ifndef PRI_FAST8
#	if INT_FAST8_WIDTH == INT_WIDTH
#	define PRI_FAST8
#	elif INT_FAST8_WIDTH == SHRT_WIDTH
#	define PRI_FAST8 "h"
#	else
#	define PRI_FAST8 "hh"
#	endif
#endif

#ifndef SCN_FAST8
#	if INT_FAST8_WIDTH == INT_WIDTH
#	define SCN_FAST8
#	elif INT_FAST8_WIDTH == SHRT_WIDTH
#	define SCN_FAST8 "h"
#	else
#	define SCN_FAST8 "hh"
#	endif
#endif

#ifndef PRIXFAST8
#define PRIXFAST8 PRI_FAST8 "X"
#endif
#ifndef PRIdFAST8
#define PRIdFAST8 PRI_FAST8 "d"
#endif
#ifndef PRIiFAST8
#define PRIiFAST8 PRI_FAST8 "d"
#endif
#ifndef PRIoFAST8
#define PRIoFAST8 PRI_FAST8 "o"
#endif
#ifndef PRIuFAST8
#define PRIuFAST8 PRI_FAST8 "u"
#endif
#ifndef PRIxFAST8
#define PRIxFAST8 PRI_FAST8 "x"
#endif
#ifndef SCNXFAST8
#define SCNXFAST8 PRI_FAST8 "X"
#endif
#ifndef SCNdFAST8
#define SCNdFAST8 SCN_FAST8 "d"
#endif
#ifndef SCNiFAST8
#define SCNiFAST8 SCN_FAST8 "d"
#endif
#ifndef SCNoFAST8
#define SCNoFAST8 SCN_FAST8 "o"
#endif
#ifndef SCNuFAST8
#define SCNuFAST8 SCN_FAST8 "u"
#endif
#ifndef SCNxFAST8
#define SCNxFAST8 SCN_FAST8 "x"
#endif

#ifndef __int_fast8_t_defined
#define __int_fast8_t_defined
typedef signed INT_FAST8_TYPE int_fast8_t;
#endif
#ifndef __uint_fast8_t_defined
#define __uint_fast8_t_defined
typedef unsigned INT_FAST8_TYPE uint_fast8_t;
#endif

#if MCC_CPU_WORD_WIDTH >= 16
#ifndef INT_FAST16_WIDTH
#ifdef __INT_FAST16_WIDTH__
#define INT_FAST16_WIDTH
#endif
#endif

#ifndef INT_FAST16_TYPE
#	ifdef __INT_FAST16_TYPE__
#	define INT_FAST16_TYPE __INT_FAST16_TYPE__
#	elif defined( INT_FAST16_WIDTH )
#		if INT_FAST16_WIDTH == INT_WIDTH
#		define INT_FAST16_TYPE int
#		elif INT_FAST16_WIDTH == SHRT_WIDTH
#		define INT_FAST16_TYPE short
#		else
#		define INT_FAST16_TYPE char
#		endif
#	else
#	define INT_FAST16_TYPE int
#	define INT_FAST16_WIDTH INT_WIDTH
#	define PRI_FAST16
#	define SCN_FAST16
#	endif
#endif

#ifndef PRI_FAST16
#	if INT_FAST16_WIDTH == INT_WIDTH
#	define PRI_FAST16
#	elif INT_FAST16_WIDTH == SHRT_WIDTH
#	define PRI_FAST16 "h"
#	else
#	define PRI_FAST16 "hh"
#	endif
#endif

#ifndef SCN_FAST16
#	if INT_FAST16_WIDTH == INT_WIDTH
#	define SCN_FAST16
#	elif INT_FAST16_WIDTH == SHRT_WIDTH
#	define SCN_FAST16 "h"
#	else
#	define SCN_FAST16 "hh"
#	endif
#endif

#ifndef PRIXFAST16
#define PRIXFAST16 PRI_FAST16 "X"
#endif
#ifndef PRIdFAST16
#define PRIdFAST16 PRI_FAST16 "d"
#endif
#ifndef PRIiFAST16
#define PRIiFAST16 PRI_FAST16 "d"
#endif
#ifndef PRIoFAST16
#define PRIoFAST16 PRI_FAST16 "o"
#endif
#ifndef PRIuFAST16
#define PRIuFAST16 PRI_FAST16 "u"
#endif
#ifndef PRIxFAST16
#define PRIxFAST16 PRI_FAST16 "x"
#endif
#ifndef SCNXFAST16
#define SCNXFAST16 PRI_FAST16 "X"
#endif
#ifndef SCNdFAST16
#define SCNdFAST16 SCN_FAST16 "d"
#endif
#ifndef SCNiFAST16
#define SCNiFAST16 SCN_FAST16 "d"
#endif
#ifndef SCNoFAST16
#define SCNoFAST16 SCN_FAST16 "o"
#endif
#ifndef SCNuFAST16
#define SCNuFAST16 SCN_FAST16 "u"
#endif
#ifndef SCNxFAST16
#define SCNxFAST16 SCN_FAST16 "x"
#endif

#ifndef __int_fast16_t_defined
#define __int_fast16_t_defined
typedef signed INT_FAST16_TYPE int_fast16_t;
#endif
#ifndef __uint_fast16_t_defined
#define __uint_fast16_t_defined
typedef unsigned INT_FAST16_TYPE uint_fast16_t;
#endif

#if MCC_CPU_WORD_WIDTH >= 32
#ifndef INT_FAST32_WIDTH
#ifdef __INT_FAST32_WIDTH__
#define INT_FAST32_WIDTH
#endif
#endif

#ifndef INT_FAST32_TYPE
#	ifdef __INT_FAST32_TYPE__
#	define INT_FAST32_TYPE __INT_FAST32_TYPE__
#	elif defined( INT_FAST32_WIDTH )
#		if INT_FAST32_WIDTH == INT_WIDTH
#		define INT_FAST32_TYPE int
#		else
#		define INT_FAST32_TYPE long
#		endif
#	elif INT_WIDTH >= 32
#	define INT_FAST32_TYPE int
#	define INT_FAST32_WIDTH INT_WIDTH
#	define PRI_FAST32
#	define SCN_FAST32
#	else
#	define INT_FAST32_TYPE long
#	define INT_FAST32_WIDTH LONG_WIDTH
#	define PRI_FAST32 "l"
#	define SCN_FAST32 "l"
#	endif
#endif

#ifndef PRI_FAST32
#	if INT_FAST32_WIDTH == INT_WIDTH
#	define PRI_FAST32
#	else
#	define PRI_FAST32 "l"
#	endif
#endif

#ifndef SCN_FAST32
#	if INT_FAST32_WIDTH == INT_WIDTH
#	define SCN_FAST32
#	else
#	define SCN_FAST32 "l"
#	endif
#endif

#ifndef PRIXFAST32
#define PRIXFAST32 PRI_FAST32 "X"
#endif
#ifndef PRIdFAST32
#define PRIdFAST32 PRI_FAST32 "d"
#endif
#ifndef PRIiFAST32
#define PRIiFAST32 PRI_FAST32 "d"
#endif
#ifndef PRIoFAST32
#define PRIoFAST32 PRI_FAST32 "o"
#endif
#ifndef PRIuFAST32
#define PRIuFAST32 PRI_FAST32 "u"
#endif
#ifndef PRIxFAST32
#define PRIxFAST32 PRI_FAST32 "x"
#endif
#ifndef SCNXFAST32
#define SCNXFAST32 PRI_FAST32 "X"
#endif
#ifndef SCNdFAST32
#define SCNdFAST32 SCN_FAST32 "d"
#endif
#ifndef SCNiFAST32
#define SCNiFAST32 SCN_FAST32 "d"
#endif
#ifndef SCNoFAST32
#define SCNoFAST32 SCN_FAST32 "o"
#endif
#ifndef SCNuFAST32
#define SCNuFAST32 SCN_FAST32 "u"
#endif
#ifndef SCNxFAST32
#define SCNxFAST32 SCN_FAST32 "x"
#endif

#ifndef __int_fast32_t_defined
#define __int_fast32_t_defined
typedef signed INT_FAST32_TYPE int_fast32_t;
#endif
#ifndef __uint_fast32_t_defined
#define __uint_fast32_t_defined
typedef unsigned INT_FAST32_TYPE uint_fast32_t;
#endif

#if MCC_CPU_WORD_WIDTH >= 64
#ifndef INT_FAST64_WIDTH
#ifdef __INT_FAST64_WIDTH__
#define INT_FAST64_WIDTH
#endif
#endif

#ifndef INT_FAST64_TYPE
#	ifdef __INT_FAST64_TYPE__
#	define INT_FAST64_TYPE __INT_FAST64_TYPE__
#	elif defined( INT_FAST64_WIDTH )
#		if INT_FAST64_WIDTH == INT_WIDTH
#		define INT_FAST64_TYPE int
#		elif INT_FAST64_WIDTH == LONG_WIDTH
#		define INT_FAST64_TYPE long
#		else
#		define INT_FAST64_TYPE long long
#		endif
#	else
#	define INT_FAST64_TYPE long long
#	define INT_FAST64_WIDTH LLONG_WIDTH
#	define PRI_FAST64 "ll"
#	define SCN_FAST64 "ll"
#	endif
#endif

#ifndef PRI_FAST64
#	if INT_FAST64_WIDTH == INT_WIDTH
#	define PRI_FAST64
#	elif INT_FAST64_WIDTH == LONG_WIDTH
#	define PRI_FAST64 "l"
#	else
#	define PRI_FAST64 "ll"
#	endif
#endif

#ifndef SCN_FAST64
#	if INT_FAST64_WIDTH == INT_WIDTH
#	define SCN_FAST64
#	elif INT_FAST64_WIDTH == LONG_WIDTH
#	define SCN_FAST64 "l"
#	else
#	define SCN_FAST64 "ll"
#	endif
#endif

#ifndef PRIXFAST64
#define PRIXFAST64 PRI_FAST64 "X"
#endif
#ifndef PRIdFAST64
#define PRIdFAST64 PRI_FAST64 "d"
#endif
#ifndef PRIiFAST64
#define PRIiFAST64 PRI_FAST64 "d"
#endif
#ifndef PRIoFAST64
#define PRIoFAST64 PRI_FAST64 "o"
#endif
#ifndef PRIuFAST64
#define PRIuFAST64 PRI_FAST64 "u"
#endif
#ifndef PRIxFAST64
#define PRIxFAST64 PRI_FAST64 "x"
#endif
#ifndef SCNXFAST64
#define SCNXFAST64 PRI_FAST64 "X"
#endif
#ifndef SCNdFAST64
#define SCNdFAST64 SCN_FAST64 "d"
#endif
#ifndef SCNiFAST64
#define SCNiFAST64 SCN_FAST64 "d"
#endif
#ifndef SCNoFAST64
#define SCNoFAST64 SCN_FAST64 "o"
#endif
#ifndef SCNuFAST64
#define SCNuFAST64 SCN_FAST64 "u"
#endif
#ifndef SCNxFAST64
#define SCNxFAST64 SCN_FAST64 "x"
#endif

#ifndef __int_fast64_t_defined
#define __int_fast64_t_defined
typedef signed INT_FAST64_TYPE int_fast64_t;
#endif
#ifndef __uint_fast64_t_defined
#define __uint_fast64_t_defined
typedef unsigned INT_FAST64_TYPE uint_fast64_t;
#endif
#endif
#endif
#endif

#endif
