#include <mcc/core/vflt.h>
#include <ctype.h>
#include <pthread.h>

mcc_vflt_t fill_big( mcc_fpn_limits_t limits,
	mcc_vflt_t big, uchar *base, size_t size ) {
	big.limits = limits;
	big.rounds = FLT_ROUNDS;
	big.epsilon = FLT_EPSILON;
	big.base = mcc_vint_wrap( 0, base, size );
	big.inf = mcc_vint_wrap( 0, &(base[size]), size );
	big.neg = mcc_vint_wrap( 0, &(base[size * 2]), size );
	big.num = mcc_vint_wrap( 0, &(base[size * 3]), size );
	big.fpn = mcc_vint_wrap( 0, &(base[size * 4]), size );
	big.one = mcc_vint_wrap( 0, &(base[size * 5]), size );
	big.tmp = mcc_vint_wrap( 0, &(base[size * 6]), size );
	big.raw = mcc_vint_wrap( 0, &(base[size * 7]), size );
	return big;
}

mcc_vflt_t mcc_vflt_set_inf( mcc_vflt_t big ) {
	int ret = mcc_vint_validate( &(big.inf) );
	mcc_fpn_limits_t limits = big.limits;
	if ( ret != EXIT_SUCCESS ) {
		printf( "Error: %08X, %s\n", ret, strerror(ret) );
	}
	memset( big.inf.zero.seg, ~0u, big.inf.size );
	mcc___vint_op_shl( big.inf, limits.exp_bits );
	mcc__vint_op_not( big.inf );
	return big;
}
int mcc_vflt_validiate( mcc_vflt_t *mcc_vflt ) {
	int ret;
	if ( !mcc_vflt ) return EDESTADDRREQ;
	ret = mcc_vint_validate( &(mcc_vflt->base) );
	if ( ret != EXIT_SUCCESS ) return ret;
	return EXIT_SUCCESS;
}
mcc_vflt_t mcc_vflt_set_raw( mcc_vflt_t mcc, mcc_vflt_t tmp ) {
	int ret = mcc_vflt_validiate(&mcc);
	mcc_vint_seg_t bits = 0;
	mcc_uhuge_t _one = 1, size, need;
	mcc_uvint_t one = {0};
	mcc_fpn_limits_t limits = mcc.limits;
	if ( ret != EXIT_SUCCESS ) {
		printf("Error: %08X, %s\n", ret, strerror(ret) );
		return mcc;
	}
	ret = mcc_vflt_validiate(&tmp);
	if ( ret != EXIT_SUCCESS ) {
		printf("Error: %08X, %s\n", ret, strerror(ret) );
		return mcc;
	}
	need = ((limits.exp_bits + limits.man_bits + 1) / CHAR_BIT);
	size = (limits.max_exp / CHAR_MAX) + 1;
	if ( size <= (need * 4) || tmp.pos > 0
		|| mcc__vint_is_nil(mcc.num)
		|| mcc__vint_is_nil(mcc.one)
		|| tmp.raw.size < need || tmp.inf.size < need
		|| tmp.neg.size < need || tmp.fpn.size < size
		|| tmp.num.size < size || tmp.one.size < size
		|| tmp.tmp.size < size || tmp.base.size < size
		|| mcc.raw.size < need || mcc.inf.size < need
		|| mcc.neg.size < need || mcc.fpn.size < size
		|| mcc.num.size < size || mcc.one.size < size
		|| mcc.tmp.size < size || mcc.base.size < size )
		goto mcc_vflt_sig;
	one = mcc_vint_wrap( 0, &_one, sizeof(_one) );
	mcc_vint_fill( tmp.tmp, tmp.num );
	mcc___vint_op_div( tmp.tmp, tmp.one, tmp.fpn );
	if ( mcc__vint_is_nil( mcc.fpn ) ) {
		(void)mcc_vint_fill( tmp.num, tmp.tmp );
		(void)mcc_vint_fill( tmp.one, one );
		tmp.pos = 0;
	}
	while ( tmp.exp > 0 ) {
		if ( tmp.pos == 0 || mcc__vint_is_eql( tmp.one, one ) )
			break;
		(void)mcc___vint_op_div( tmp.one, tmp.base, tmp.tmp );
		tmp.pos++;
		tmp.exp--;
	}
	if ( tmp.exp < limits.min_exp_value ) goto mcc_vflt_sig;
	if ( tmp.exp > limits.max_exp_value ) {
		mcc_vflt_inf:
		mcc_vint_fill( tmp.raw, mcc.inf );
		goto mcc_vflt_exp;
	}
	for ( ; tmp.exp > 0; tmp.exp-- ) {
		if ( mcc___vint_op_mul( tmp.num, tmp.base, tmp.tmp ) ==
			EOVERFLOW ) goto mcc_vflt_inf;
	}
	for ( ; tmp.exp < 0; tmp.exp++ ) {
		if ( mcc___vint_op_mul( tmp.one, tmp.base, tmp.tmp ) ==
			EOVERFLOW ) goto mcc_vflt_sig;
	}
	if ( mcc__vint_is_nil( tmp.one ) ) goto mcc_vflt_sig;
	(void)mcc___vint_op_div( tmp.num, tmp.one, tmp.fpn );
	if ( mcc__vint_is_nil( tmp.fpn ) ) mcc_vint_fill( tmp.one, one );
	tmp.pos = 0;
	if ( mcc__vint_is_gte( tmp.num, one ) ) {
		for (
			mcc_vint_fill( tmp.tmp, tmp.num );
			mcc__vint_is_gth( tmp.tmp, one );
			tmp.pos++, mcc___vint_op_shr( tmp.tmp, 1, 0 )
		);
	}
	else {
		for (
			mcc_vint_fill( tmp.tmp, tmp.one );
			mcc__vint_is_gth( tmp.tmp, tmp.fpn );
			tmp.pos--, mcc___vint_op_shr( tmp.tmp, 1, 0 )
		);
		if ( mcc__vint_is_eql( tmp.tmp, tmp.fpn ) ) tmp.pos--;
	}
	mcc.pos = tmp.pos;
	mcc_vint_fill(
		tmp.raw, mcc_vint_wrap(
			1, &(limits.exp_bias), sizeof(limits.exp_bias) ) );
	if ( mcc__vint_is_gth( tmp.raw, mcc.inf ) ) goto mcc_vflt_inf;
	if ( tmp.pos > limits.man_bits ) {
		tmp.pos -= limits.man_bits;
		mcc__vint_op_xor( tmp.fpn, tmp.fpn );
		mcc__vint_op_not( tmp.fpn );
		mcc___vint_op_shl( tmp.fpn, tmp.pos );
		mcc__vint_op_not( tmp.fpn );
		mcc__vint_op_and( tmp.fpn, tmp.num );
		mcc___vint_op_shl( tmp.one, tmp.pos );
		mcc_vint_fill( tmp.tmp, tmp.num );
		mcc___vint_op_shr( tmp.tmp, tmp.pos, 0 );
		mcc_vint_fill( mcc.raw, tmp.tmp );
		mcc_vint_fill( tmp.tmp, tmp.one );
		mcc___vint_op_shr( tmp.tmp, 1, 0 );
		ret = mcc__vint_op_cmp( tmp.fpn, tmp.tmp );
		if ( ret > 0 ) mcc__vint_op_inc( mcc.raw );
		else if ( ret == 0 ) {
			switch ( mcc.rounds ) {
			case 1:
				if ( *(mcc.raw.zero.seg) & mcc.raw.zero.bit )
					mcc__vint_op_inc( mcc.raw );
				break;
			}
		}
	}
	else {
		mcc_vint_fill( mcc.raw, tmp.num );
		mcc_vint_fill( tmp.tmp, tmp.fpn );
		for ( ; tmp.pos < limits.man_bits; tmp.pos++ ) {
			bits++;
			mcc___vint_op_shl( tmp.tmp, 1 );
			if ( mcc__vint_is_gte( tmp.tmp, tmp.one ) ) {
				(void)mcc__vint_op_sub( tmp.tmp, tmp.one );
				(void)mcc___vint_op_shl( tmp.num, bits );
				*(tmp.num.zero.seg) |= tmp.num.zero.bit;
				bits = 0;
			}
		}
		if ( bits ) (void)mcc___vint_op_shl( tmp.num, bits );
	}
	bits = (mcc.raw.stop.b - mcc.raw.zero.b) - limits.man_bits;
	mcc___vint_op_shl( mcc.raw, bits );
	mcc___vint_op_shr( mcc.raw, bits, 0 );
	mcc_vflt_exp:
	mcc___vint_op_shl( tmp.raw, limits.exp_bits );
	mcc__vint_op_aor( mcc.raw, tmp.raw );
	mcc_vflt_sig:
	mcc___vint_op_shl( tmp.neg, limits.man_bits + limits.exp_bits );
	return mcc;
}
mcc_vflt_t mcc_vflt_literal( char *txt, mcc_vflt_t tmp, mcc_vflt_t big ) {
	char *text, sig;
	int c = 0, l = 10, h = 10;
	char e = 'e', E = 'E';
	struct mcc__vint C = mcc_vint_wrap( 1, &c, sizeof(c) );
	while ( isspace(*txt) ) ++txt;
	text = txt;
	(void)mcc__vint_op_xor( tmp.neg, tmp.neg );
	switch ( *txt ) {
		case '-': *(tmp.neg.zero.seg) = tmp.neg.zero.bit;
		case '+': ++txt;
	}
	if ( *txt == '0' ) {
		++txt;
		switch ( *txt ) {
		case 'b': case 'B':
			++txt;
			c = 2;
			mcc_vint_fill( tmp.base, C );
			break;
		case 'x': case 'X':
			++txt;
			c = 16;
			mcc_vint_fill( tmp.base, C );
			break;
		}
		c = 36;
		if ( mcc__vint_is_gth( tmp.base, C ) )
			l = 36;
	}
	while ( *txt ) {
		c = mcc_geti( *txt, l, h );
		if ( c < 0 || mcc__vint_is_gte( C, tmp.base ) ) break;
		++txt;
		if ( mcc___vint_op_mul( tmp.num, tmp.base, tmp.tmp )
			== EOVERFLOW ) {
			(void)mcc_vint_fill( big.raw, big.neg );
			(void)mcc___vint_op_shl( big.raw, big.limits.exp_bits );
			(void)mcc__vint_op_aor( big.raw, big.inf );
			(void)mcc___vint_op_shl( big.raw, big.limits.man_bits );
			return big;
		}
		(void)mcc__vint_op_add( tmp.num, C );
	}
	tmp.pos = 0;
	if ( *txt == '.' ) {
		++txt;
		while ( *txt ) {
			c = mcc_geti( *txt, l, h );
			if ( c < 0 || mcc__vint_is_gte( C, tmp.base ) ) break;
			++txt;
			if ( mcc___vint_op_mul( tmp.num, tmp.base, tmp.tmp )
				== EOVERFLOW ) {
				(void)mcc_vint_fill( big.raw, big.neg );
				(void)mcc___vint_op_shl( big.raw, big.limits.exp_bits );
				(void)mcc___vint_op_shl( big.raw, big.limits.man_bits );
				return big;
			}
			(void)mcc__vint_op_add( tmp.num, C );
			(void)mcc___vint_op_mul( tmp.one, tmp.base, tmp.tmp );
			tmp.pos--;
		}
	}
	if ( *txt == e || *txt == E ) {
		++txt;
		switch ( *txt ) { case '-': case '+': sig = *txt; ++txt; }
		while ( *txt ) {
			c = mcc_geti( *txt, l, h );
			if ( c < 0 || mcc__vint_is_gte( C, tmp.base ) ) break;
			++txt;
			tmp.exp *= *(tmp.base.zero.seg);
			tmp.exp += c;
		}
		if ( sig == '-' ) tmp.exp = -(tmp.exp);
	}
	mcc_vint_fill( C, tmp.base );
	mcc_vint_fill( big.inf, tmp.inf );
	mcc_vint_fill( big.num, tmp.num );
	mcc_vint_fill( big.fpn, tmp.fpn );
	mcc_vint_fill( big.one, tmp.one );
	return mcc_vflt_set_raw( big, tmp );
}
