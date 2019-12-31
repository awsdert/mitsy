#ifndef INC_MCC_FPN_H
#define INC_MCC_FPN_H

#include <float.h>
#include <mcc/core/vint.h>
#include <mcc/bitsof.h>
#if 1
#define mcc_huge __int128
#define MCC_UHUGE_MAX ((unsigned __int128)-1)
#define MCC_HUGE_MAX ((__int128)(MCC_UHUGE_MAX >> 1))
#define MCC_HUGE_MIN ((__int128)~(MCC_UHUGE_MAX >> 1))
#else
#define mcc_huge long long
#define MCC_UHUGE_MAX ULLONG_MAX
#define MCC_HUGE_MAX LLONG_MAX
#define MCC_HUGE_MIN LLONG_MIN
#endif
typedef signed mcc_huge mcc_huge_t;
typedef unsigned mcc_huge mcc_uhuge_t;

#ifdef HALF_MANT_DIG
#define HALF_PRIa "%ha"
#define HALF_PRIf "%hf"
#define HALF_PRIe "%he"
#define HALF_SCNa "%ha"
#define HALF_SCNf "%hf"
#define HALF_SCNe "%he"
#define HALF_MAN_BIT (HALF_MANT_DIG - 1)
#define HALF_EXP_BIT (bitsof(half) - HALF_MANT_DIG)
#endif

#define FLT_PRIa "%a"
#define FLT_PRIf "%f"
#define FLT_PRIe "%e"
#define FLT_SCNa "%a"
#define FLT_SCNf "%f"
#define FLT_SCNe "%e"
#define FLT_MAN_BIT (FLT_MANT_DIG - 1)
#define FLT_EXP_BIT (bitsof(float) - FLT_MANT_DIG)

#define DBL_PRIa "%la"
#define DBL_PRIf "%lf"
#define DBL_PRIe "%le"
#define DBL_SCNa "%la"
#define DBL_SCNf "%lf"
#define DBL_SCNe "%le"
#define DBL_MAN_BIT (DBL_MANT_DIG - 1)
#define DBL_EXP_BIT (bitsof(double) - DBL_MANT_DIG)

#define LDBL_PRIa "%lla"
#define LDBL_PRIf "%llf"
#define LDBL_PRIe "%lle"
#define LDBL_SCNa "%lla"
#define LDBL_SCNf "%llf"
#define LDBL_SCNe "%lle"
#define LDBL_MAN_BIT (LDBL_MANT_DIG - 1)
#define LDBL_EXP_BIT (bitsof(long double) - LDBL_MANT_DIG)

typedef struct mcc_fpn_limits {
	ulong exp_bias;
	long man_bits;
	long exp_bits;
	long max_exp;
	long min_exp;
	long max_exp_value;
	long min_exp_value;
} mcc_fpn_limits_t;

typedef struct mcc_vflt {
	long rounds;
	long double epsilon;
	long pos;
	long exp;
	struct mcc__vint inf;
	struct mcc__vint base;
	struct mcc__vint neg;
	struct mcc__vint num;
	struct mcc__vint fpn;
	struct mcc__vint one;
	struct mcc__vint tmp;
	struct mcc__vint raw;
	mcc_fpn_limits_t limits;
} mcc_vflt_t;

mcc_vflt_t mcc_vflt_set_inf( mcc_vflt_t mcc );
mcc_vflt_t mcc_vflt_set_raw( mcc_vflt_t tmp, mcc_vflt_t ret );
mcc_vflt_t mcc_vflt_literal( char *txt, mcc_vflt_t tmp, mcc_vflt_t ret );

#endif
