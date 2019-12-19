#ifndef INC_BITSOF_H
#define INC_BITSOF_H
#include <mcc/stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#ifndef __bitsof_defined
#define __bitsof_defined
#define bitsof(T) (sizeof(T) * CHAR_BIT)
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

#ifndef __xlong_defined
#if defined( __int128_defined ) && !defined( __int128_extension )
#define __xlong_defined
#define xlong __int128
#define PRI_XLONG "I128"
#define SCN_XLONG "I128"
#define UXLONG_MAX (~0UI128)
#define XLONG_MAX ((~0I128) >> 1)
#define XLONG_MIN ((-XLONG_MAX)-1)
#elif defined( MCC_HAVE_LLONG )
#define __xlong_defined
#define xlong long long int
#define PRI_XLONG "ll"
#define SCN_XLONG "ll"
#define UXLONG_MAX ULLONG_MAX
#define XLONG_MAX LLONG_MAX
#define XLONG_MIN LLONG_MIN
#elif defined( __int64_defined ) && !defined( __int64_extension )
#define __xlong_defined
#define xlong __int64
#define PRI_XLONG "I64"
#define SCN_XLONG "I64"
#define UXLONG_MAX UINT64_MAX
#define XLONG_MAX INT64_MAX
#define XLONG_MIN INT64_MIN
#elif SIZEOF_LONG >= 8
#define __xlong_defined
#define xlong long int
#define PRI_XLONG "l"
#define SCN_XLONG "l"
#define UXLONG_MAX ULONG_MAX
#define XLONG_MAX LONG_MAX
#define XLONG_MIN LONG_MIN
#endif
#endif

#ifndef __uxlong_defined
typedef unsigned xlong uxlong;
#endif

#endif /* INC_BITSOF_H */
