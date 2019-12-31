#ifndef INC_MCC_TEST_CORE_VFLT_H
#define INC_MCC_TEST_CORE_VFLT_H

#include "vint.h"
#include <mcc/core/vflt.h>

typedef union mcc_flt_union
{
	float fpn;
	mcc_uhuge_t raw;
	struct
	{
		mcc_uhuge_t man:FLT_MAN_BIT;
		mcc_uhuge_t exp:FLT_EXP_BIT;
		mcc_uhuge_t sig:1;
	};
} mcc_flt_union_t;

typedef union mcc_dbl_union
{
	double fpn;
	mcc_uhuge_t raw;
	struct
	{
		mcc_uhuge_t man:DBL_MAN_BIT;
		mcc_uhuge_t exp:DBL_EXP_BIT;
		mcc_uhuge_t sig:1;
	};
} mcc_dbl_union_t;

typedef union mcc_ldbl_union
{
	long double fpn;
	mcc_uhuge_t raw;
	struct
	{
		mcc_uhuge_t man:LDBL_MAN_BIT;
		mcc_uhuge_t exp:LDBL_EXP_BIT;
		mcc_uhuge_t sig:1;
	};
} mcc_ldbl_union_t;

int test_vflt();

#endif
