#ifndef MCC_STDINT_H
#define MCC_STDINT_H

#ifndef MCC_IS_ROOT_INC
#ifdef MCC_SYS_IS_WINDOWS
#include <BaseTsd.h>
#elif defined( MCC_SYS_LINUX )
#include <linux/stdint.h>
#else
#include <stdint.h>
#endif
#endif

#include "limits.h"

#ifdef MCC_SYS_LLP64
#define MCC_INTPTR_TYPE long long
#define MCC_UINTPTR_TYPE unsigned long long
#define MCC_PRI_INTPTR "ll"
#define MCC_SCN_INTPTR "ll"
#else
#define MCC_INTPTR_TYPE long
#define MCC_UINTPTR_TYPE unsigned long
#define MCC_PRI_INTPTR "l"
#define MCC_SCN_INTPTR "l"
#endif

#define MCC_PRI_LEAST8
#define MCC_SCN_LEAST8
#define MCC_INT_LEAST8_TYPE int
#define MCC_UINT_LEAST8_TYPE unsigned int

#define MCC_PRI_LEAST16
#define MCC_SCN_LEAST16
#define MCC_INT_LEAST16_TYPE int
#define MCC_UINT_LEAST16_TYPE unsigned int

#if ((~0u) >> 31)
#define MCC_PRI_LEAST32
#define MCC_SCN_LEAST32
#define MCC_INT_LEAST32_TYPE int
#define MCC_UINT_LEAST32_TYPE unsigned int
#elif ((~0ul) >> 31)
#define MCC_PRI_LEAST32 "l"
#define MCC_SCN_LEAST32 "l"
#define MCC_INT_LEAST32_TYPE long int
#define MCC_UINT_LEAST32_TYPE unsigned long int
#endif

#ifdef MCC_HAVE_LLONG
#define MCC_PRI_LEAST64 "ll"
#define MCC_SCN_LEAST64 "ll"
#define MCC_INT_LEAST64_TYPE long long int
#define MCC_UINT_LEAST64_TYPE unsigned long long int
#elif ((~0ul) >> 63)
#define MCC_PRI_LEAST64 "l"
#define MCC_SCN_LEAST64 "l"
#define MCC_INT_LEAST64_TYPE long int
#define MCC_UINT_LEAST64_TYPE unsigned long int
#endif

#define MCC__PRI_FAST8
#define MCC__SCN_FAST8
#define MCC__INT_FAST8_TYPE int
#define MCC__UINT_FAST8_TYPE unsigned int

#define MCC__PRI_FAST16
#define MCC__SCN_FAST16
#define MCC__INT_FAST16_TYPE int
#define MCC__UINT_FAST16_TYPE unsigned int

#if ((~0u) >> 31)
#define MCC__PRI_FAST32
#define MCC__SCN_FAST32
#define MCC__INT_FAST32_TYPE int
#define MCC__UINT_FAST32_TYPE unsigned int
#elif ((~0ul) >> 31)
#define MCC__PRI_FAST32 "l"
#define MCC__SCN_FAST32 "l"
#define MCC__INT_FAST32_TYPE long int
#define MCC__UINT_FAST32_TYPE unsigned long int
#endif

#ifdef MCC__HAVE_LLONG
#define MCC__PRI_FAST64 "ll"
#define MCC__SCN_FAST64 "ll"
#define MCC__INT_FAST64_TYPE long long int
#define MCC__UINT_FAST64_TYPE unsigned long long int
#elif ((~0ul) >> 63)
#define MCC__PRI_FAST64 "l"
#define MCC__SCN_FAST64 "l"
#define MCC__INT_FAST64_TYPE long int
#define MCC__UINT_FAST64_TYPE unsigned long int
#endif

#ifdef MCC_SYS_WINDOWS_OS
#include <BaseTsd.h>
#else
#include <inttypes.h>
#endif

#ifndef __int_least8_t_defined
#define __int_least8_t_defined
#ifdef __INT8_TYPE__
#define PRIX8 __PRIX8__
#define PRId8 __PRId8__
#define PRIo8 __PRIo8__
#define PRIu8 __PRIu8__
#define PRIx8 __PRIx8__
#define SCNX8 __SCNX8__
#define SCNd8 __SCNd8__
#define SCNo8 __SCNo8__
#define SCNu8 __SCNu8__
#define SCNx8 __SCNx8__
typedef __INT8_TYPE__ int8_t;
#elif CHAR_WIDTH == 8
#define PRIX8 "X"
#define PRId8 "d"
#define PRIo8 "o"
#define PRIu8 "u"
#define PRIx8 "x"
#define SCNX8 "hhX"
#define SCNd8 "hhd"
#define SCNo8 "hho"
#define SCNu8 "hhu"
#define SCNx8 "hhx"
typedef signed char int8_t;
#elif INT_WIDTH == 8
#define PRIX8 "X"
#define PRId8 "d"
#define PRIo8 "o"
#define PRIu8 "u"
#define PRIx8 "x"
#define SCNX8 "X"
#define SCNd8 "d"
#define SCNo8 "o"
#define SCNu8 "u"
#define SCNx8 "x"
typedef signed int int8_t;
#endif
#endif

#endif
