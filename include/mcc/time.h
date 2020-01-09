#ifndef INC_MCC_TIME_H
#define INC_MCC_TIME_H

#include <mcc/bitsof.h>

#ifndef MCC_IS_ROOT_INC
#if defined( MCC_SYS_LINUX )
#include <linux/time.h>
#else
#include <time.h>
#endif
#endif

#ifndef TIME32_TYPE
#ifdef __TIME32_TYPE__
#define TIME32_TYPE __TIME32_TYPE__
#else
#define TIME32_TYPE uint_least32_t
#endif
#endif

#ifndef CLOCK32_TYPE
#ifdef __CLOCK32_TYPE__
#define CLOCK32_TYPE __CLOCK32_TYPE__
#else
#define CLOCK32_TYPE uint_least32_t
#endif
#endif

#ifndef TIME64_TYPE
#ifdef __TIME64_TYPE__
#define TIME64_TYPE __TIME64_TYPE__
#elif defined( __uint_least64_t_defined )
#define TIME64_TYPE uint_least64_t
#endif
#endif

#ifndef CLOCK64_TYPE
#ifdef __CLOCK64_TYPE__
#define CLOCK64_TYPE __CLOCK64_TYPE__
#elif defined( __uint_least64_t_defined )
#define CLOCK64_TYPE uint_least64_t
#endif
#endif

#ifndef __time32_t_defined
#define __time32_t_defined
typedef TIME32_TYPE time32_t;
#endif

#ifndef __clock32_t_defined
#define __clock32_t_defined
typedef CLOCK32_TYPE clock32_t;
#endif

#ifdef __uint_least64_t_defined
#ifndef __time64_t_defined
#define __time64_t_defined
typedef TIME64_TYPE time64_t;
#endif

#ifndef __clock64_t_defined
#define __clock64_t_defined
typedef CLOCK64_TYPE clock64_t;
#endif
#endif

#ifndef __time_t_defined
#define __time_t_defined
#ifdef __time64_t_defined
typedef time64_t time_t;
#else
typedef time32_t time_t;
#endif
#endif

#ifndef __clock_t_defined
#define __clock_t_defined
#ifdef __clock64_t_defined
typedef clock64_t clock_t;
#else
typedef clock32_t clock_t;
#endif
#endif

#ifdef MCC_IS_ROOT_INC
#define CLOCK_REALTIME           0
#define CLOCK_MONOTONIC          1
#define CLOCK_PROCESS_CPUTIME_ID 2
#define CLOCK_THREAD_CPUTIME_ID  3
#define CLOCK_MONOTONIC_RAW      4
#define CLOCK_REALTIME_COARSE    5
#define CLOCK_MONOTONIC_COARSE   6
#define CLOCK_BOOTTIME           7
#define CLOCK_REALTIME_ALARM     8
#define CLOCK_BOOTTIME_ALARM     9
#define CLOCK_SGI_CYCLE         10
#define CLOCK_TAI               11
clock_t clock();
time_t time( time_t * _timer );
#endif

typedef ptrdiff_t mcc_rnd_t;
long mcc__rnd( mcc_rnd_t *ctx, long min, long max );
#define mcc_rnd( ctx ) mcc__rnd( ctx, LONG_MIN, LONG_MAX )

#endif
