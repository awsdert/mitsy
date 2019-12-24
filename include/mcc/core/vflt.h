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
typedef union FLT_UNION
{
	float fpn;
	mcc_uhuge_t raw;
	struct
	{
		mcc_uhuge_t man:FLT_MAN_BIT;
		mcc_uhuge_t exp:FLT_EXP_BIT;
		mcc_uhuge_t sig:1;
	};
} FLT_UNION;

#define DBL_PRIa "%la"
#define DBL_PRIf "%lf"
#define DBL_PRIe "%le"
#define DBL_SCNa "%la"
#define DBL_SCNf "%lf"
#define DBL_SCNe "%le"
#define DBL_MAN_BIT (DBL_MANT_DIG - 1)
#define DBL_EXP_BIT (bitsof(double) - DBL_MANT_DIG)
typedef union DBL_UNION
{
	double fpn;
	mcc_uhuge_t raw;
	struct
	{
		mcc_uhuge_t man:DBL_MAN_BIT;
		mcc_uhuge_t exp:DBL_EXP_BIT;
		mcc_uhuge_t sig:1;
	};
} DBL_UNION;

#define LDBL_PRIa "%lla"
#define LDBL_PRIf "%llf"
#define LDBL_PRIe "%lle"
#define LDBL_SCNa "%lla"
#define LDBL_SCNf "%llf"
#define LDBL_SCNe "%lle"
#define LDBL_MAN_BIT (LDBL_MANT_DIG - 1)
#define LDBL_EXP_BIT (bitsof(long double) - LDBL_MANT_DIG)
typedef union LDBL_UNION
{
	long double fpn;
	mcc_uhuge_t raw;
	struct
	{
		mcc_uhuge_t man:LDBL_MAN_BIT;
		mcc_uhuge_t exp:LDBL_EXP_BIT;
		mcc_uhuge_t sig:1;
	};
} LDBL_UNION;

#define FPN_PFX(VAL) DBL##_##VAL
#define FPN_MAX FPN_PFX(_MAX)
#define FPN_PRIa FPN_PFX(PRIa)
#define FPN_PRIf FPN_PFX(PRIf)
#define FPN_PRIe FPN_PFX(PRIe)
#define FPN_SCNa FPN_PFX(SCNa)
#define FPN_SCNf FPN_PFX(SCNf)
#define FPN_SCNe FPN_PFX(SCNe)
#define FPN_DIG FPN_PFX(DIG)
#define FPN_EPSILON FPN_PFX(EPSILON)
#define FPN_MAN_BIT FPN_PFX(MAN_BIT)
#define FPN_MANT_DIG FPN_PFX(MANT_DIG)
#define FPN_EXP_BIT FPN_PFX(EXP_BIT)
#define FPN_MAX_EXP FPN_PFX(MAX_EXP)
#define FPN_MIN_EXP FPN_PFX(MIN_EXP)
#define FPN_MAX_10_EXP FPN_PFX(MAX_10_EXP)
#define FPN_MIN_10_EXP FPN_PFX(MIN_10_EXP)
typedef FPN_PFX(UNION) FPN_UNION;

typedef struct mcc_fpn_limits {
	ulong exp_bias;
	long man_bits;
	long exp_bits;
	long max_exp;
	long min_exp;
	long max_exp_digits;
	long min_exp_digits;
} mcc_fpn_limits_t;

typedef struct mcc_fpn {
	long rounds;
	long double epsilon;
	long pos;
	long exp;
	mcc_uhuge_t base;
	mcc_uhuge_t inf;
	mcc_uhuge_t neg;
	mcc_uhuge_t num;
	mcc_uhuge_t fpn;
	mcc_uhuge_t one;
	mcc_uhuge_t tmp;
	mcc_uhuge_t raw;
	mcc_fpn_limits_t limits;
} mcc_fpn_t;

typedef struct mcc_big {
	long rounds;
	long double epsilon;
	long pos;
	long exp;
	mcc_uvint_t inf;
	mcc_uvint_t base;
	mcc_uvint_t neg;
	mcc_uvint_t num;
	mcc_uvint_t fpn;
	mcc_uvint_t one;
	mcc_uvint_t tmp;
	mcc_uvint_t raw;
	mcc_fpn_limits_t limits;
} mcc_big_t;

#endif
