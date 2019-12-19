#ifndef MCC_LIMITS_H
#define MCC_LIMITS_H

#include <mcc/core/sys.h>

#ifndef MCC_IS_ROOT_INC
#ifdef MCC_SYS_WINDOWS_OS
#include <BaseTsd.h>
#elif defined( MCC_SYS_LINUX )
#include <linux/limits.h>
#else
#include <limits.h>
#endif
#endif

#ifdef MCC_STDC_VER
#if MCC_STDC_VER >= 199900l
#define MCC_HAVE_LLONG
#endif
#elif defined( MCC_SYS_LLP64 ) || defined( __mitsy__ )
#define MCC_HAVE_LLONG
#elif defined( __LLONG_MIN__ ) && __LLONG_MIN__ < 0
#define MCC_HAVE_LLONG
#elif defined( __LONG_LONG_MIN__ ) && __LONG_LONG_MIN__ < 0
#define MCC_HAVE_LLONG
#elif defined( __GNUC__ ) && __GNUC__ >= 3L
#define MCC_HAVE_LLONG
#endif

#ifdef MCC_STDC_VER
#if MCC_STDC_VER >= 201100l
#define MCC_HAVE_UTF_LITERALS
#endif
#elif defined( __mitsy__ )
#define MCC_HAVE_UTF_LITERALS
#endif

#ifndef CHAR_BIT
#	ifdef __CHAR_BIT__
#	define CHAR_BIT __CHAR_BIT__
#	elif defined( CHAR_WIDTH )
#	define CHAR_BIT CHAR_WIDTH
#	elif defined( __CHAR_WIDTH__ )
#	define CHAR_BIT __CHAR_WIDTH__
#	else
#	define CHAR_BIT MCC_CPU_CHAR_WIDTH
#	endif
#endif

#ifdef MCC_HAVE_LLONG
#	ifndef SIZEOF_LLONG
#		ifdef __SIZEOF_LLONG__
#		define SIZEOF_LLONG __SIZEOF_LLONG__
#		elif defined( __SIZEOF_LONG_LONG__ )
#		define SIZEOF_LLONG __SIZEOF_LONG_LONG__
#		else
#		define SIZEOF_LLONG 8
#		endif
#	endif
#	ifndef LLONG_WIDTH
#		ifdef __LLONG_WIDTH__
#		define LLONG_WIDTH __LLONG_WIDTH__
#		elif defined( __LONG_LONG_WIDTH__ )
#		define LLONG_WIDTH __LONG_LONG_WIDTH__
#		else
#		define LLONG_WIDTH (SIZEOF_LLONG * CHAR_BIT)
#		endif
#	endif
#	ifndef ULLONG_MAX
#		ifdef __ULLONG_MAX__
#		define ULLONG_MAX __ULLONG_MAX__
#		elif defined( __ULONG_LONG_MAX__ )
#		define ULLONG_MAX __ULONG_LONG_MAX__
#		else
#		define ULLONG_MAX (~0ull)
#		endif
#	endif
#	ifndef LLONG_MAX
#		ifdef __LLONG_MAX__
#		define LLONG_MAX __LLONG_MAX__
#		elif defined( __LONG_LONG_MAX__ )
#		define LLONG_MAX __LONG_LONG_MAX__
#		else
#		define LLONG_MAX (~(~0ll << (LLONG_WIDTH - 1)))
#		endif
#	endif
#	ifndef LLONG_MIN
#		ifdef __LLONG_MIN__
#		define LLONG_MIN __LLONG_MIN__
#		elif defined( __LONG_LONG_MIN__ )
#		define LLONG_MIN __LONG_LONG_MIN__
#		elif ((-LLONG_MAX)-1) < (-LLONG_MAX)
#		define LLONG_MIN ((-LLONG_MAX)-1)
#		else
#		define LLONG_MIN (-LLONG_MAX)
#		endif
#	endif
#endif

#ifndef SIZEOF_LONG
#	ifdef __SIZEOF_LONG__
#	define SIZEOF_LONG __SIZEOF_LONG__
/* 128bit etc will be caught above since they're not common yet
 * but the above is already implemented by gcc, mitsy will have it
 * out of the box and windows defaults to 32bit for backward
 * compatibility reasons, vc will likely use __int128 for integers &
 * pointers that big */
#	elif (~0ul) >> 63
#	define SIZEOF_LONG 8
#	else
#	define SIZEOF_LONG 4
#	endif
#endif

#ifndef LONG_WIDTH
#	ifdef __LONG_WIDTH__
#	define LONG_WIDTH __LONG_WIDTH__
#	else
#	define LONG_WIDTH (SIZEOF_LONG * CHAR_BIT)
#	endif
#endif

#ifndef ULONG_MAX
#	if defined( __ULONG_MAX__ ) && __ULONG_MAX__ > 0
#	define ULONG_MAX __ULONG_MAX__
#	else
#	define ULONG_MAX (~0ul)
#	endif
#endif
#ifndef LONG_MAX
#	if defined( __LONG_MAX__ ) && __LONG_MAX__ > 0
#	define LONG_MAX __LONG_MAX__
#	else
#	define LONG_MAX (~(~0l << (LONG_WIDTH - 1)))
#	endif
#endif
#ifndef LONG_MIN
#	if defined( __LONG_MIN__ ) && __LONG_MIN__ < 0
#	define LONG_MIN __LONG_MIN__
#	elif ((-LONG_MAX)-1) < (-LONG_MAX)
#	define LONG_MIN ((-LONG_MAX)-1)
#	else
#	define (-LONG_MAX)
#	endif
#endif

#ifndef SIZEOF_INT
#	ifdef __SIZEOF_INT__
#	define SIZEOF_INT __SIZEOF_INT__
#	elif (~0u) >> 63
#	define SIZEOF_INT 8
#	elif (~0u) >> 31
#	define SIZEOF_INT 4
#	else
#	define SIZEOF_INT 2
#	endif
#endif

#ifndef INT_WIDTH
#	ifdef __INT_WIDTH__
#	define INT_WIDTH __INT_WIDTH__
#	else
#	define INT_WIDTH (SIZEOF_INT * CHAR_BIT)
#	endif
#endif

#ifndef INT_MAX
#	ifndef INT_MAX
#		ifdef __UINT_MAX__
#		define UINT_MAX __UINT_MAX__
#		else
#		define UINT_MAX (~0u)
#		endif
#	endif
#	ifndef INT_MAX
#		ifdef __INT_MAX__
#		define INT_MAX __INT_MAX__
#		else
#		define INT_MAX (~(~0 << (INT_WIDTH - 1)))
#		endif
#	endif
#	ifndef INT_MIN
#		if defined( __INT_MIN__ ) && __INT_MIN__ < 0
#		define INT_MIN __INT_MIN__
#		elif ((-INT_MAX)-1) < (-INT_MAX)
#		define INT_MIN ((-INT_MAX)-1)
#		else
#		define INT_MIN (-INT_MAX)
#		endif
#	endif
#endif

#ifndef SIZEOF_SHRT
#	ifdef __SIZEOF_SHRT__
#	define SIZEOF_SHRT __SIZEOF_SHRT__
#	elif defined( __SIZEOF_SHORT__ )
#	define SIZEOF_SHRT __SIZEOF_SHORT__
#	elif defined( MCC_SYS_SILP64 )
#	define SIZEOF_SHRT 8
#	elif defined( MCC_SYS_SILP32 )
#	define SIZEOF_SHRT 4
#	else
#	define SIZEOF_SHRT 2
#	endif
#endif

#ifndef SHRT_WIDTH
#	ifdef __SHRT_WIDTH__
#	define SHRT_WIDTH __SHRT_WIDTH__
#	elif defined( __SHORT_WIDTH__ )
#	define SHRT_WIDTH __SHORT_WIDTH__
#	else
#	define SHRT_WIDTH (SIZEOF_SHRT * CHAR_BIT)
#	endif
#endif

#ifndef USHRT_MAX
#	ifdef __USHRT_MAX__
#	define USHRT_MAX __USHRT_MAX__
#	elif defined( __USHORT_MAX__ )
#	define USHRT_MAX __USHORT_MAX__
#	else
#	define USHRT_MAX (~(~0u << SHRT_WIDTH))
#	endif
#endif
#ifdef SHRT_MAX
#	ifdef __SHRT_MAX__
#	define SHRT_MAX __SHRT_MAX__
#	else
#	define SHRT_MAX (~(~0 << (SHRT_WIDTH - 1)))
#	endif
#endif
#ifndef SHRT_MIN
#	ifdef __SHRT_MIN__
#	define SHRT_MIN __SHRT_MIN__
#	elif INT_MIN < (-INT_MAX)
#	define SHRT_MIN ((-SHRT_MAX)-1)
#	else
#	define SHRT_MIN (-SHRT_MAX)
#	endif
#endif

#ifndef SIZEOF_CHAR
#	define SIZEOF_CHAR 1
#endif

#ifndef CHAR_WIDTH
#	define CHAR_WIDTH CHAR_BIT
#endif

#ifndef UCHAR_MAX
#	ifdef __UCHAR_MAX__
#	define UCHAR_MAX __UCHAR_MAX__
#	else
#	define UCHAR_MAX (~(~0u << CHAR_WIDTH))
#	endif
#endif
#ifdef SCHAR_MAX
#	ifdef __SCHAR_MAX__
#	define SCHAR_MAX __SCHAR_MAX__
#	else
#	define SCHAR_MAX (~(~0 << (CHAR_WIDTH - 1)))
#	endif
#endif
#ifndef SCHAR_MIN
#	ifdef __SCHAR_MIN__
#	define SCHAR_MIN __SCHAR_MIN__
#	elif INT_MIN < (-INT_MAX)
#	define SCHAR_MIN ((-SCHAR_MAX)-1)
#	else
#	define SCHAR_MIN (-SCHAR_MAX)
#	endif
#endif

#ifndef UNSIGNED_CHAR
#ifdef __UNSIGNED_CHAR__
#define UNSIGNED_CHAR
#endif
#endif
#ifndef SIGNED_CHAR
#ifdef __SIGNED_CHAR__
#define SIGNED_CHAR
#endif
#endif

#ifndef CHAR_MAX
#ifdef UNSIGNED_CHAR
#define CHAR_MAX UCHAR_MAX
#else
#define CHAR_MAX SCHAR_MAX
#endif
#endif
#ifndef CHAR_MIN
#ifdef UNSIGNED_CHAR
#define CHAR_MAX 0
#else
#define CHAR_MAX SCHAR_MIN
#endif
#endif

#endif
