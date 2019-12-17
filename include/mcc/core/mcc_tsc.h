#ifndef INC_MCC_TSC_H
#define INC_MCC_TSC_H
#include "mcc_err.h"
#include "mcc_bitsof.h"
#include <time.h>

#if defined(__ARM__)
#define MCC_CPU_ARM
#endif

#if defined(__AMD64__)
#define MCC_CPU_AMD64
#endif

#if defined(__i386__)
#define MCC_CPU_i386
#endif

#if defined(MCC_CPU_ARM) || defined(MCC_CPU_AMD64) || defined(__x86_64__)
#define MCC_CPU_x86_64
#endif

#if defined(MCC_CPU_i386) || defined(MCC_CPU_x86_64) || defined(__x86__)
#define MCC_CPU_x86
#endif

#if defined( __SSE2__ )
#define MCC_CPU_SSE2
#endif

#if !defined(MCC_CPU_x86)
#define MCC_CPU_OTHER
#endif

typedef volatile ullong mcc_tsc_t;
mcc_tsc_t mcc_tsc();

#endif
