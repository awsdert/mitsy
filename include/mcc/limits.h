#ifndef MCC_LIMITS_H
#define MCC_LIMITS_H

#include <mcc/sys.h>

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

#ifdef MCC_HAVE_LLONG
#	ifndef LLONG_MAX
#		if defined( __ULLONG_MAX__ ) && __ULLONG_MAX__ > 0
#		define ULLONG_MAX __ULLONG_MAX__
#		elif defined( __ULONG_LONG_MAX__ ) && __ULONG_LONG_MAX__ > 0
#		define ULLONG_MAX __ULONG_LONG_MAX__
#		else
#		define ULLONG_MAX (~0ull)
#		endif
#	endif
#	ifndef LLONG_MAX
#		if defined( __LLONG_MAX__ ) && __LLONG_MAX__ > 0
#		define LLONG_MAX __LLONG_MAX__
#		elif defined( __LONG_LONG_MAX__ ) && __LONG_LONG_MAX__ > 0
#		define LLONG_MAX __LONG_LONG_MAX__
#		else
#		define LLONG_MAX (0ll | (ULLONG_MAX >> 1))
#		endif
#	endif
#	ifndef LLONG_MIN
#		if defined( __LLONG_MIN__ ) && __LLONG_MIN__ < 0
#		define LLONG_MIN __LLONG_MIN__
#		elif defined( __LONG_LONG_MIN__ ) && __LONG_LONG_MIN__ < 0
#		define LLONG_MIN __LONG_LONG_MIN__
#		else
#		define LLONG_MIN ((-LLONG_MAX)-1)
#		endif
#	endif
#endif

#ifndef LONG_MAX
#	ifndef LONG_MAX
#		if defined( __ULONG_MAX__ ) && __ULONG_MAX__ > 0
#		define ULONG_MAX __ULONG_MAX__
#		else
#		define ULONG_MAX (~0ul)
#		endif
#	endif
#	ifndef LONG_MAX
#		if defined( __LONG_MAX__ ) && __LONG_MAX__ > 0
#		define LONG_MAX __LONG_MAX__
#		else
#		define LONG_MAX (0l | (ULONG_MAX >> 1))
#		endif
#	endif
#	ifndef LONG_MIN
#		if defined( __LONG_MIN__ ) && __LONG_MIN__ < 0
#		define LONG_MIN __LONG_MIN__
#		else
#		define LONG_MIN ((-LONG_MAX)-1)
#		endif
#	endif
#endif

#ifndef INT_MAX
#	ifndef INT_MAX
#		if defined( __UINT_MAX__ ) && __UINT_MAX__ > 0
#		define UINT_MAX __UINT_MAX__
#		else
#		define UINT_MAX (~0u)
#		endif
#	endif
#	ifndef INT_MAX
#		if defined( __INT_MAX__ ) && __INT_MAX__ > 0
#		define INT_MAX __INT_MAX__
#		else
#		define INT_MAX (0 | (UINT_MAX >> 1))
#		endif
#	endif
#	ifndef INT_MIN
#		if defined( __INT_MIN__ ) && __INT_MIN__ < 0
#		define INT_MIN __INT_MIN__
#		else
#		define INT_MIN ((-INT_MAX)-1)
#		endif
#	endif
#endif

#ifdef __SHRT_MAX__
#define MCC_SHRT_MAX __SHRT_MAX__
#define MCC_SHRT_MIN __SHRT_MIN__
#define MCC_USHRT_MAX __USHRT_MAX__
#elif defined( __SHORT_MAX__ )
#define MCC_SHRT_MAX __SHORT_MAX__
#define MCC_SHRT_MIN __SHORT_MIN__
#define MCC_USHRT_MAX __USHORT_MAX__
#elif defined( MCC_SYS_SILP64 ) || MCC_SYS_SILP32 )
#define MCC_SHRT_MAX MCC_INT_MAX
#define MCC_SHRT_MIN MCC_INT_MIN
#define MCC_USHRT_MAX MCC_UINT_MAX
#elif defined( MCC_STDC_VER ) || ((~0u) >> 31)
#define MCC_SHRT_MAX 32767
#define MCC_SHRT_MIN -32768
#define MCC_USHRT_MAX 65535u
#else
#define MCC_SHRT_MAX MCC_INT_MAX
#define MCC_SHRT_MIN MCC_INT_MIN
#define MCC_USHRT_MAX MCC_UINT_MAX
#endif

#ifdef __SCHAR_MAX__
#define MCC_SCHAR_MAX __SCHAR_MAX__
#define MCC_SCHAR_MIN __SCHAR_MIN__
#define MCC_UCHAR_MAX __UCHAR_MAX__
#elif defined( MCC_STDC_VER ) || ((MCC_CPU_INT_WIDTH % 8) == 0)
#define MCC_SCHAR_MAX 127
#define MCC_SCHAR_MIN -127
#define MCC_UCHAR_MAX 255u
#else
#define MCC_SCHAR_MAX MCC_INT_MAX
#define MCC_SCHAR_MIN MCC_INT_MIN
#define MCC_UCHAR_MAX MCC_UINT_MAX
#endif

#ifdef __CHAR_WIDTH__
#define MCC_CHAR_WIDTH __CHAR_WIDTH__
#elif defined( __CHAR_BIT__ )
#define MCC_CHAR_WIDTH __CHAR_BIT__
#elif defined( MCC_STDC_VER )
#define MCC_CHAR_WIDTH 8
#else
#define MCC_CHAR_WIDTH 4
#endif

#ifdef MCC_HAVE_LLONG
#ifdef __LLONG_WIDTH__
#define MCC_LLONG_WIDTH __LLONG_WIDTH__
#elif defined( __LLONG_BIT__ )
#define MCC_LLONG_WIDTH __LLONG_BIT__
#elif defined( __LONG_LONG_WIDTH__ )
#define MCC_LLONG_WIDTH __LONG_LONG_WIDTH__
#elif defined( __LONG_LONG_BIT__ )
#define MCC_LLONG_WIDTH __LONG_LONG_BIT__
#endif

#endif
