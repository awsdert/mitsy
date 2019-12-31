#ifndef INC_MCC_VINT_H
#define INC_MCC_VINT_H

#include <mcc/bitsof.h>
#include <mcc/core/get.h>

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define INC_BITMATH
//#define MCC__INT_USE_HUGE

typedef unsigned int mcc_test_t;
#define MCC_TEST_END_BIT (UINT_MAX & INT_MIN)
#define MCC_TEST_PRI_PFX ""
#ifndef MCC__INT_USE_HUGE
//#define MCC__INT_USE_TEST
#define MCC__INT_USE_CHAR
#endif
#ifdef MCC__INT_USE_TEST
typedef mcc_test_t mcc_vint_seg_t;
#define MCC_VINT_SEG_END_BIT (~0u & MCC_TEST_END_BIT)
#define MCC_VINT_SEG_PRI_PFX MCC_TEST_PRI_PFX
#elif defined( MCC__INT_USE_CHAR )
typedef unsigned char mcc_vint_seg_t;
#define MCC_VINT_SEG_END_BIT (UCHAR_MAX & SCHAR_MIN)
#define MCC_VINT_SEG_PRI_PFX ""
#elif defined( MCC__INT_USE_HUGE )
typedef unsigned __int128 mcc_vint_seg_t;
#define MCC_VINT_SEG_END_BIT (~((~((unsigned __int128)0u)) >> 1))
#define MCC_VINT_SEG_PRI_PFX "I128"
#else
typedef unsigned long mcc_vint_seg_t;
#define MCC_VINT_SEG_END_BIT (~(ULONG_MAX >> 1))
#define MCC_VINT_SEG_PRI_PFX "l"
#endif

typedef struct mcc_bit {
	mcc_vint_seg_t i;
	mcc_vint_seg_t *seg;
	mcc_vint_seg_t b;
	mcc_vint_seg_t bit;
} mcc_bit_t;

int mcc_bit_op_cmp( mcc_bit_t num, mcc_bit_t val );
mcc_bit_t mcc_bit_op_inc( mcc_bit_t num );
mcc_bit_t mcc_bit_op_dec( mcc_bit_t num );
mcc_bit_t mcc__bit_op_add( mcc_bit_t num, mcc_vint_seg_t bits );
mcc_bit_t mcc__bit_op_sub( mcc_bit_t num, mcc_vint_seg_t bits );
mcc_bit_t mcc_bit_op_dec_for_bit( mcc_bit_t stop, mcc_bit_t zero );

typedef struct mcc__vint {
	int exit;
	size_t size;
	mcc_bit_t zero, stop, sign;
	mcc_vec_t buff;
} mcc_vint_t, mcc_uvint_t;

/** @brief Converts a character to it's integer equivalent
 *	@param character the character returned by getc() or similar
 *	@param lowercase added to result of character - 'a'
 *	@param uppercase added to result of character - 'A'
 *  @return -1 on failure or integer equivalent of the character
**/
int mcc_geti( long character, long lowercase, long uppercase );
int mcc__vint_printb( mcc_vint_t num );
int mcc__vint_scanb( mcc_vint_t num );
int mcc__vint_get_sign( struct mcc__vint num );
int mcc_vint_validate( struct mcc__vint *num );
int mcc_vint_validate2( struct mcc__vint *num, struct mcc__vint *val );
int mcc_vint_validate3( struct mcc__vint *num, struct mcc__vint *val, struct mcc__vint *var );
void mcc_vint_print( struct mcc__vint num, mcc_vint_seg_t min );
struct mcc__vint mcc_vint_wrap( bool has_sign, void *addr, size_t size );
int mcc_vint_size( struct mcc__vint *num, size_t size );
int mcc_vint_fill( struct mcc__vint num, struct mcc__vint val );
int mcc_vint_to_val( struct mcc__vint num, intmax_t val );
int mcc_vint_to_uval( struct mcc__vint num, uintmax_t val );
int mcc_vint_size_and_fill( struct mcc__vint *num, struct mcc__vint val );
int mcc__vint_op_cmp( struct mcc__vint num, struct mcc__vint val );
#define mcc__vint_is_nil( num ) (mcc__vint_op_cmp( num, (struct mcc__vint){0} ) == 0)
#define mcc__vint_is_eql( num, val ) (mcc__vint_op_cmp( num, val ) == 0)
#define mcc__vint_is_neq( num, val ) (mcc__vint_op_cmp( num, val ) != 0)
#define mcc__vint_is_gth( num, val ) (mcc__vint_op_cmp( num, val ) >  0)
#define mcc__vint_is_gte( num, val ) (mcc__vint_op_cmp( num, val ) >= 0)
#define mcc__vint_is_lth( num, val ) (mcc__vint_op_cmp( num, val ) <  0)
#define mcc__vint_is_lte( num, val ) (mcc__vint_op_cmp( num, val ) <= 0)
int mcc__vint_op_not( struct mcc__vint num );
int mcc__vint_op_inc( struct mcc__vint num );
int mcc__vint_op_dec( struct mcc__vint num );
int mcc__vint_op_aor( struct mcc__vint num, struct mcc__vint val );
int mcc__vint_op_xor( struct mcc__vint num, struct mcc__vint val );
int mcc__vint_op_and( struct mcc__vint num, struct mcc__vint val );
int mcc___vint_op_shl( struct mcc__vint num, mcc_vint_seg_t bits );
int mcc___vint_op_shr( struct mcc__vint num, mcc_vint_seg_t bits, bool neg );
int mcc__vint_op_shl( struct mcc__vint num, struct mcc__vint val );
int mcc__vint_op_shr( struct mcc__vint num, struct mcc__vint val, bool neg );
int mcc__vint_op_add( struct mcc__vint num, struct mcc__vint val );
int mcc___vint_op_mul( struct mcc__vint num, struct mcc__vint val, struct mcc__vint tmp );
int mcc__vint_op_mul( struct mcc__vint num, struct mcc__vint val );
int mcc__vint_op_sub( struct mcc__vint num, struct mcc__vint val );
int mcc___vint_op_div( struct mcc__vint num, struct mcc__vint val, struct mcc__vint rem );
int mcc__vint_op_div( struct mcc__vint num, struct mcc__vint val );
int mcc__vint_op_mod( struct mcc__vint num, struct mcc__vint val );

#endif
