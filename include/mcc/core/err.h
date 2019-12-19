#ifndef INC_FAIL_H
#define INC_FAIL_H
/* TODO: Provide implementations for MCC_IS_ROOT_INC scenario, musl is
 * best library to refer to for ideas on such stuff */
#ifdef MCC_SYS_LINUX
#include <linux/errno.h>
#include <linux/stdlib.h>
#include <linux/stdio.h>
#include <linux/string.h>
#else
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif

#define ERRORPOS_STR "%s:%d:"
#define ERRORPOS_VAL __FILE__, __LINE__

#define FUNCTION_STR "%s():"
#if defined __GNUP__
#define FUNCTION_VAL __PRETTY_FUNCTION__
#elif defined __GNUC__
#define FUNCTION_VAL __FUNCTION__
#elif defined _Pragma
#define FUNCTION_VAL __func__
#else
#define FUNCTION_VAL "???"
#endif

#define FAIL( OUT, ERR, INF )                                          \
	fprintf( OUT, ERRORPOS_STR "%s(): Error: 0x%08X, %d, %s, Info: %s\n",\
		ERRORPOS_VAL, FUNCTION_VAL, ERR, ERR, strerror(ERR), INF )

#define INFO( OUT, ERR, INF )                                          \
	fprintf( OUT, ERRORPOS_STR "%s(): Info: 0x%08X, %d, %s, %s\n",\
		ERRORPOS_VAL, FUNCTION_VAL, ERR, ERR, strerror(ERR), INF )

#define BAIL( OUT, ERR, INF ) FAIL( OUT, ERR, INF ); exit(EXIT_FAILURE)

#endif
