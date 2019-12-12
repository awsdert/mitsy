#ifndef INC_BITSOF_H
#define INC_BITSOF_H
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>

#ifndef __bitsof_defined
#define __bitsof_defined
#define bitsof(T) (sizeof(T) * CHAR_BIT)
#endif

#if !defined( ULLONG_MAX ) && defined( ULONG_LONG_MAX )
#define ULLONG_MAX ULONG_LONG_MAX
#endif
#if !defined( LLONG_MAX ) && defined( LONG_LONG_MAX )
#define LLONG_MAX LONG_LONG_MAX
#endif
#if !defined( LLONG_MIN ) && defined( LONG_LONG_MIN )
#define LLONG_MIN LONG_LONG_MIN
#endif

#ifndef HAVE_LLONG
/* Make absolute certain long long int is available */
#	if defined( LLONG_MIN ) && LLONG_MIN < 0
#	define HAVE_LLONG
#	endif
#endif

#ifndef __schar_defined
#define __schar_defined
typedef signed char schar;
#endif
#ifndef __uchar_defined
#define __uchar_defined
typedef unsigned char uchar;
#endif

#ifdef HAVE_LLONG
#	ifndef __sllong_defined
#	define __sllong_defined
typedef signed long long sllong;
#	endif
#	ifndef __ullong_defined
#	define __ullong_defined
typedef unsigned long long ullong;
#	endif
#endif

#ifndef __int64_extension
#	ifdef __mitsy__
#		if __INTMAX_WIDTH__ < 64
#		define __int64_extension
#		endif
#	elif defined( __MSC__ )
#		if defined( __i386__ )
#		define __int64_extension
#		endif
#	endif
#endif

#ifndef __int128_extension
#	ifdef __mitsy__
#		if __INTMAX_WIDTH__ < 128
#		define __int128_extension
#		endif
#	elif defined( __GNUC__ ) && defined( __GNUC_MINOR__ )
#		if __GNUC__ > 5 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#		define __int128_extension
#		endif
#	endif
#endif

#ifndef SIZEOF_SHRT
#ifdef __SIZEOF_SHRT__
#define SIZEOF_SHRT __SIZEOF_SHRT__
#elif defined( __SHRT_SIZE__ )
#define SIZEOF_SHRT __SHRT_SIZE__
#elif SHRT_MAX == CHAR_MAX
#define SIZEOF_SHRT 1
#elif SHRT_MAX < INT_MAX
#define SIZEOF_SHRT 2
#elif SHRT_MAX < LONG_MAX
#if !defined( LLONG_MAX ) || LLONG_MAX > LONG_MAX
#define SIZEOF_SHRT 2
#else
#define SIZEOF_SHRT 8
#endif
#endif

#ifndef __xlong_defined
#if defined( __int128_defined ) && !defined( __int128_extension )
#define xlong __int128
#define PRI_XLONG "I128"
#define SCN_XLONG "I128"
#define UXLONG_MAX (~0UI128)
#define XLONG_MAX ((~0I128) >> 1)
#define XLONG_MIN ((-XLONG_MAX)-1)
#if defined( LLONG_MIN ) && LLONG_MIN < 0
#define xlong long long int
#define PRI_XLONG "ll"
#define SCN_XLONG "ll"
#define UXLONG_MAX ULLONG_MAX
#define XLONG_MAX LLONG_MAX
#define XLONG_MIN LLONG_MIN
#elif defined( __int64_defined ) && !defined( __int64_extension )
#define xlong __int64
#define PRI_XLONG "I64"
#define SCN_XLONG "I64"
#elif (LONG_MAX > INT_MAX) && (INT_MAX > SHRT_MAX)
#define xlong long int
#define PRI_XLONG "l"
#define SCN_XLONG "l"
#endif
#endif

#ifndef __uxlong_defined
typedef unsigned xlong uxlong;
#endif

#endif /* INC_BITSOF_H */
