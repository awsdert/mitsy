#include <mcc/core/vint.h>

int mcc_geti( long c, long l, long h ) {
	if ( c >= '0' && c <= '9' )
		return c - '0';
	else if ( c >= 'A' && c <= 'Z' )
		return (c - 'A') + h;
	else if ( c >= 'a' && c <= 'z' )
		return (c - 'a') + l;
	return -1;
}

mcc_bit_t mcc_bit_op_inc( mcc_bit_t num ) {
	mcc_vint_seg_t max = 0;
	max = ~max;
	if ( num.b == max ) {
		memset( &num, 0, sizeof(num) );
		return num;
	}
	++(num.b);
	num.bit <<= 1;
	if ( !(num.bit) ) {
		num.bit = 1;
		++(num.i);
		++(num.seg);
	}
	return num;
}

mcc_bit_t mcc_bit_op_dec( mcc_bit_t num ) {
	if ( !num.b ) {
		memset( &num, 0, sizeof(num) );
		return num;
	}
	--(num.b);
	num.bit >>= 1;
	if ( !(num.bit) ) {
		num.bit = MCC_VINT_SEG_END_BIT;
		--(num.i);
		--(num.seg);
	}
	return num;
}
mcc_bit_t mcc_bit_op_dec_for_bit( mcc_bit_t stop, mcc_bit_t zero ) {
	while ( stop.b > zero.b ) {
		stop = mcc_bit_op_dec(stop);
		if ( *(stop.seg) & stop.bit )
			return stop;
	}
	return stop;
}
//#define MCC_BIT_SLOW
mcc_bit_t mcc__bit_op_add( mcc_bit_t num, mcc_vint_seg_t bits ) {
#ifdef MCC_BIT_SLOW
	while ( bits-- ) {
		num = mcc_bit_op_inc(num);
		if ( !(num.bit) ) return num;
	}
	return num;
#else
	mcc_vint_seg_t i = num.bit;
	if ( i == 1u ) goto mcc__bit_op_add_bytes;
	while ( bits && i ) {
		--bits; i <<= 1u;
		num = mcc_bit_op_inc(num);
		if ( !(num.bit) ) return num;
	}
	mcc__bit_op_add_bytes:
	if ( !bits ) return num;
	i = num.b;
	num.b += bits;
	if ( num.b < i ) {
		memset( &num, 0, sizeof(num) );
		return num;
	}
	i = bits / bitsof(i);
	num.i += i;
	num.seg += i;
	num.bit = 1u << (bits %= bitsof(i));
	return num;
#endif
}

mcc_bit_t mcc__bit_op_sub( mcc_bit_t num, mcc_vint_seg_t bits ) {
#ifdef MCC_BIT_SLOW
	while ( bits-- ) {
		num = mcc_bit_op_dec(num);
		if ( !(num.bit) ) return num;
	}
	return num;
#else
	mcc_vint_seg_t i = num.bit;
	if ( i == MCC_VINT_SEG_END_BIT ) goto mcc__bit_op_sub_bytes;
	while ( bits && i ) {
		--bits;
		i >>= 1;
		num = mcc_bit_op_dec( num );
		if ( !(num.bit) ) return num;
	}
	mcc__bit_op_sub_bytes:
	if ( !bits ) return num;
	i = num.b;
	num.b -= bits;
	if ( num.b > i ) {
		memset( &num, 0, sizeof(num) );
		return num;
	}
	i = bits / bitsof(i);
	num.i -= i;
	num.seg -= i;
	num.bit >>= (bits % bitsof(i));
	return num;
#endif
}

int mcc_bit_op_cmp( mcc_bit_t num, mcc_bit_t val ) {
	if ( num.b > val.b ) return 1;
	if ( num.b < val.b ) return -1;
	return 0;
}

int mcc_vint_validate( struct mcc__vint *num ) {
	if ( !num || !(num->zero.seg) || !(num->stop.seg) )
		return EADDRNOTAVAIL;
	if ( ( num->exit == EXIT_SUCCESS || num->exit ==  EOVERFLOW )
		&& mcc_bit_op_cmp( num->zero, num->stop ) > 0 )
		num->exit = ERANGE;
	return num->exit;
}
int mcc_vint_validate2( struct mcc__vint *num, struct mcc__vint *val ) {
	int ret = mcc_vint_validate( num );
	switch ( ret ) {
	case EXIT_SUCCESS: break;
	case EADDRNOTAVAIL: return EDESTADDRREQ;
	default: return ret;
	}
	return mcc_vint_validate( val );
}

int mcc_vint_validate3( struct mcc__vint *num, struct mcc__vint *val, struct mcc__vint *var ) {
	int ret = mcc_vint_validate2( num, val );
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = mcc_vint_validate(var);
	if ( ret != EXIT_SUCCESS )
		return (ret == EADDRNOTAVAIL) ? EDESTADDRREQ : ret;
	if ( var->size < num->size ) return ERANGE;
	return EXIT_SUCCESS;
}
int mcc__vint_printb( struct mcc__vint num ) {
	int ret = mcc_vint_validate( &num );
	mcc_bit_t n = {0};
	if ( ret != EXIT_SUCCESS ) return ret;
	for ( n = num.stop; n.b > num.zero.b; n = mcc_bit_op_dec(n) )
		putc('0' + (*(n.seg) & n.bit),stdout);
	putc('0' + (*(n.seg) & n.bit),stdout);
	return ret;
}
int mcc__vint_scanb( struct mcc__vint num ) {
	int ret = mcc__vint_op_xor( num, num ), c, b;
	mcc_bit_t n = {0};
	if ( ret != EXIT_SUCCESS ) return ret;
	for ( n = num.stop; n.b > num.zero.b; n = mcc_bit_op_dec(n) ) {
		c = getc(stdin);
		b = mcc_geti( getc(stdin), 10, 10 );
		if ( b < 0 || b > 1 ) { ungetc(c,stdin); break; }
		*(n.seg) |= n.bit;
	}
	return mcc___vint_op_shr( num, n.b - num.zero.b, 0 );
}

void mcc_vint_print( struct mcc__vint num, mcc_vint_seg_t min ) {
	int ret = mcc_vint_validate(&num);
	mcc_vint_seg_t nb;
	if ( ret != EXIT_SUCCESS ) return;
	nb = num.stop.b - num.zero.b;
	while ( min-- > nb ) putchar('0');
	while ( nb-- ) {
		num.stop = mcc_bit_op_dec(num.stop);
		putchar('0' + !!(*(num.stop.seg) & num.stop.bit) );
	}
}

struct mcc__vint mcc_vint_wrap( bool has_sign, void *addr, size_t size ) {
	struct mcc__vint tmp = {0};
	if ( !addr || !size ) return tmp;
	tmp.size = size;
	tmp.zero.seg = addr;
	tmp.zero.bit = 1;
	tmp.stop = mcc__bit_op_add( tmp.zero, size * CHAR_BIT );
	if ( has_sign ) tmp.sign = mcc_bit_op_dec( tmp.stop );
	return tmp;
}

int mcc_vint_size( struct mcc__vint *num, size_t size ) {
	mcc_vint_seg_t *mem;
	size_t nodes;
	if ( !size ) {
		if ( num->zero.seg ) free( num->zero.seg );
		memset( num, 0, sizeof(struct mcc__vint) );
		return EXIT_SUCCESS;
	}
	nodes = size / sizeof(mcc_vint_seg_t);
	if ( size % sizeof(mcc_vint_seg_t) ) ++nodes;
	size = nodes * sizeof(mcc_vint_seg_t);
	if ( !(num->zero.seg) ) mem = malloc( size );
	else mem = realloc( num->zero.seg, size );
	if ( !mem ) ENOMEM;
	(void)memset( &(num->zero), 0, sizeof(mcc_bit_t) );
	num->zero.seg = mem;
	num->zero.bit = 1;
	num->size = size;
	num->stop = mcc__bit_op_add( num->zero, size * CHAR_BIT );
	return EXIT_SUCCESS;
}
int mcc__vint_get_sign( mcc_vint_t num ) {
	if ( num.sign.seg )
		return (*(num.sign.seg) & num.sign.bit) ? 1 : 0;
	return -1;
}
int mcc_vint_fill( struct mcc__vint num, struct mcc__vint val ) {
	(void)mcc__vint_op_xor( num, num );
	if ( mcc__vint_get_sign(val) == 1 ) {
		(void)mcc__vint_op_not( num );
		return mcc__vint_op_and( num, val );
	}
	return mcc__vint_op_aor( num, val );
}
int mcc_vint_to_val( struct mcc__vint num, intmax_t val ) {
	return mcc_vint_fill( num, mcc_vint_wrap( 1, &val, sizeof(val) ) );
}
int mcc_vint_to_uval( struct mcc__vint num, uintmax_t val ) {
	return mcc_vint_fill( num, mcc_vint_wrap( 0, &val, sizeof(val) ) );
}

int mcc_vint_size_and_fill( struct mcc__vint *num, struct mcc__vint val ) {
	int ret = mcc_vint_size( num, val.size );
	if ( ret != EXIT_SUCCESS ) return ret;
	return mcc_vint_fill( *num, val );
}

int mcc__vint_op_cmp( struct mcc__vint num, struct mcc__vint val ) {
	mcc_bit_t n = {0}, v = {0};
	mcc_vint_seg_t nb = 0, vb = 0;
	int nn0 = 0, vn0 = 0;
	if ( mcc_vint_validate( &num ) == EXIT_SUCCESS ) {
		n = mcc_bit_op_dec_for_bit(num.stop, num.zero);
		nn0 = (*(n.seg) & n.bit) ? 1 : 0;
		nb = n.b - num.zero.b;
	}
	if ( mcc_vint_validate( &val ) == EXIT_SUCCESS ) {
		v = mcc_bit_op_dec_for_bit(val.stop, val.zero);
		vn0 = (*(v.seg) & v.bit) ? 1 : 0;
		vb = v.b - val.zero.b;
	}
	if ( !(n.bit) ) return -vn0;
	if ( !(v.bit) ) return nn0;
	if ( nb != vb ) return (nb > vb) ? 1 : -1;
	while ( nb ) {
		if ( nb ) {
			n = mcc_bit_op_dec_for_bit( n, num.zero );
			nb = n.b - num.zero.b;
		}
		if ( vb ) {
			v = mcc_bit_op_dec_for_bit( v, val.zero );
			vb = v.b - val.zero.b;
		}
		if ( nb != vb ) return ( nb > vb ) ? 1 : -1;
	}
	nb = (*(n.seg) & n.bit) ? 1 : 0;
	vb = (*(v.seg) & v.bit) ? 1 : 0;
	if ( nb != vb )	return (nb > vb) ? 1 : -1;
	return 0;
}

int biclamp1( struct mcc__vint num ) {
	int ret = mcc_vint_validate( &num );
	mcc_bit_t one, pos;
	if ( ret != EXIT_SUCCESS ) return ret;
	one = mcc_bit_op_dec(pos = num.stop);
	while ( pos.i == one.i ) {
		if ( !(*(pos.seg) & pos.bit) )
			*(pos.seg) |= pos.bit;
	}
	return EXIT_SUCCESS;
}
int biclamp0( struct mcc__vint num ) {
	int ret = mcc_vint_validate( &num );
	mcc_bit_t one, pos;
	if ( ret != EXIT_SUCCESS ) return ret;
	one = mcc_bit_op_dec(pos = num.stop);
	while ( pos.i == one.i ) {
		if ( *(pos.seg) & pos.bit )
			*(pos.seg) ^= pos.bit;
	}
	return EXIT_SUCCESS;
}
int mcc__vint_op_not( struct mcc__vint num ) {
	int ret = mcc_vint_validate( &num );
	mcc_bit_t n;
	if ( ret != EXIT_SUCCESS ) return ret;
	n = num.zero;
	while ( n.b < num.stop.b ) {
		*(n.seg) ^= n.bit;
		n = mcc_bit_op_inc(n);
	}
	return EXIT_SUCCESS;
}
int mcc__vint_op_aor( struct mcc__vint num, struct mcc__vint val ) {
	int ret = mcc_vint_validate2( &num, &val );
	mcc_bit_t n, v, e;
	if ( ret != EXIT_SUCCESS ) return ret;
	n = num.zero;
	v = val.zero;
	e = (mcc_bit_op_cmp(num.stop, val.stop) < 0) ? num.stop : val.stop;
	while ( n.b < e.b && v.b < e.b ) {
		if ( *(v.seg) & v.bit )
			*(n.seg) |= n.bit;
		n = mcc_bit_op_inc(n);
		v = mcc_bit_op_inc(v);
	}
	return EXIT_SUCCESS;
}
int mcc__vint_op_xor( struct mcc__vint num, struct mcc__vint val ) {
	int ret = mcc_vint_validate2( &num, &val );
	mcc_bit_t n, v, e;
	if ( ret != EXIT_SUCCESS ) return ret;
	n = num.zero;
	v = val.zero;
	e = (mcc_bit_op_cmp(num.stop, val.stop) < 0) ? num.stop : val.stop;
	while ( n.b < e.b && v.b < e.b ) {
		if ( *(v.seg) & v.bit )
			*(n.seg) ^= n.bit;
		n = mcc_bit_op_inc(n);
		v = mcc_bit_op_inc(v);
	}
	return EXIT_SUCCESS;
}
int mcc__vint_op_and( struct mcc__vint num, struct mcc__vint val ) {
	int ret = mcc_vint_validate2( &num, &val );
	mcc_bit_t n, v, e;
	if ( ret != EXIT_SUCCESS ) return ret;
	n = num.zero;
	v = val.zero;
	e = (mcc_bit_op_cmp(num.stop, val.stop) < 0) ? num.stop : val.stop;
	while ( n.b < e.b && v.b < e.b ) {
		if ( !(*(v.seg) & v.bit) && *(n.seg) & n.bit )
			*(n.seg) ^= n.bit;
		n = mcc_bit_op_inc(n);
		v = mcc_bit_op_inc(v);
	}
	while ( n.b < num.stop.b ) {
		if ( *(n.seg) & n.bit )
			*(n.seg) ^= n.bit;
		n = mcc_bit_op_inc(n);
	}
	return EXIT_SUCCESS;
}
int mcc___vint_op_shl( struct mcc__vint num, mcc_vint_seg_t bits ) {
	int ret = mcc_vint_validate( &num );
	mcc_bit_t n, v;
	mcc_vint_seg_t max_bits;
	if ( ret != EXIT_SUCCESS ) return ret;
	max_bits = (num.stop.b - num.zero.b);
	if ( bits >= max_bits )
		bits %= max_bits;
	if ( !bits ) return EXIT_SUCCESS;
	n = num.stop;
	v = mcc__bit_op_sub( n, bits );
	while ( v.b > num.zero.b ) {
		n = mcc_bit_op_dec(n);
		v = mcc_bit_op_dec(v);
		if ( *(v.seg) & v.bit )
			*(n.seg) |= n.bit;
		else if ( *(n.seg) & n.bit )
			*(n.seg) ^= n.bit;
	}
	while ( n.b > num.zero.b ) {
		n = mcc_bit_op_dec(n);
		if ( *(n.seg) & n.bit ) {
			*(n.seg) ^= n.bit;
			ret = EOVERFLOW;
		}
	}
	return ret;
}
int mcc___vint_op_shr( struct mcc__vint num, mcc_vint_seg_t bits, bool neg ) {
	int ret = mcc_vint_validate( &num );
	mcc_bit_t n, v;
	mcc_vint_seg_t max_bits;
	if ( ret != EXIT_SUCCESS ) return ret;
	max_bits = (num.stop.b - num.zero.b);
	if ( bits >= max_bits )
		bits %= max_bits;
	if ( !bits ) return EXIT_SUCCESS;
	n = num.zero;
	v = mcc__bit_op_add( n, bits );
	while ( v.b < num.stop.b ) {
		if ( *(v.seg) & v.bit )
			*(n.seg) |= n.bit;
		else if ( neg )
			*(n.seg) |= n.bit;
		else if ( *(n.seg) & n.bit )
			*(n.seg) ^= n.bit;
		n = mcc_bit_op_inc(n);
		v = mcc_bit_op_inc(v);
	}
	while ( n.b < num.stop.b ) {
		if ( *(n.seg) & n.bit ) {
			ret = EOVERFLOW;
			*(n.seg) ^= n.bit;
		}
		n = mcc_bit_op_inc(n);
	}
	return ret;
}
int mcc__vint_op_inc( struct mcc__vint num ) {
	mcc_bit_t n;
	_Bool c = 1;
	int ret = mcc_vint_validate( &num );
	if ( ret != EXIT_SUCCESS ) return ret;
	n = num.zero;
	for ( ; n.b < num.stop.b; n = mcc_bit_op_inc(n) ) {
		*(n.seg) ^= n.bit;
		if ( *(n.seg) & n.bit ) {
			c = 0;
			break;
		}
	}
	return c ? EOVERFLOW : EXIT_SUCCESS;
}
int mcc__vint_op_add( struct mcc__vint num, struct mcc__vint val ) {
	mcc_bit_t n, v, stop;
	mcc_vint_seg_t b = 0, bits, nb, vb;
	_Bool c = 0;
	int ret = mcc_vint_validate2( &num, &val );
	if ( ret != EXIT_SUCCESS ) return ret;
	n = num.zero;
	v = val.zero;
	nb = num.stop.b - num.zero.b;
	vb = val.stop.b - val.zero.b;
	bits = (nb < vb) ? nb : vb;
	stop = (mcc_bit_op_cmp(num.stop,val.stop) < 0) ? num.stop : val.stop;
	for ( ; b < bits; ++b ) {
		if ( c ) {
			*(n.seg) ^= n.bit;
			if ( *(n.seg) & n.bit )
				c = 0;
		}
		if ( *(v.seg) & v.bit ) {
			*(n.seg) ^= n.bit;
			if ( !(*(n.seg) & n.bit) )
				c = 1;
		}
		n = mcc_bit_op_inc(n);
		v = mcc_bit_op_inc(v);
	}
	if ( c ) {
		for ( ; n.b < stop.b; n = mcc_bit_op_inc(n) ) {
			*(n.seg) ^= n.bit;
			if ( *(n.seg) & n.bit ) {
				c = 0;
				break;
			}
		}
	}
	return c ? EOVERFLOW : EXIT_SUCCESS;
}
int mcc_uvint_op_add( mcc_uvint_t num, struct mcc__vint val ) {
	int ret = mcc__vint_op_add( num, val );
	if ( ret == EOVERFLOW ) {
		mcc__vint_op_xor( num, num );
		mcc__vint_op_not( num );
		return EXIT_SUCCESS;
	}
	return ret;
}
int mcc_vint_op_add( mcc_vint_t num, mcc_vint_t val ) {
	int ret = mcc_vint_validate2( &num, &val );
	struct mcc__vint tmp;
	if ( ret != EXIT_SUCCESS ) return ret;
	tmp = num;
	tmp.stop = mcc_bit_op_dec(tmp.stop);
	if ( *(tmp.stop.seg) & tmp.stop.bit )
		return mcc__vint_op_add( num, val );
	ret = mcc__vint_op_add( tmp, val );
	if ( ret == EOVERFLOW ) {
		mcc__vint_op_xor( num, num );
		mcc__vint_op_not( tmp );
		return EXIT_SUCCESS;
	}
	return ret;
}
int mcc__vint_op_dec( struct mcc__vint num ) {
	mcc_bit_t n;
	_Bool c = 1;
	int ret = mcc_vint_validate( &num );
	if ( ret != EXIT_SUCCESS ) return ret;
	n = num.zero;
	for ( ; n.b < num.stop.b; n = mcc_bit_op_inc(n) ) {
		*(n.seg) ^= n.bit;
		if ( !(*(n.seg) & n.bit) ) {
			c = 0;
			break;
		}
	}
	return c ? EOVERFLOW : EXIT_SUCCESS;
}
int mcc__vint_op_sub( struct mcc__vint num, struct mcc__vint val ) {
	mcc_bit_t n, v, s;
	_Bool c = 0;
	mcc_vint_seg_t b = 0, bits = 0, nb, vb;
	int ret = mcc_vint_validate2( &num, &val );
	if ( ret != EXIT_SUCCESS ) return ret;
	n = num.zero;
	v = val.zero;
	s = val.stop;
	nb = num.stop.b - num.zero.b;
	vb = s.b - val.zero.b;
	bits = (nb < vb) ? nb : vb;
	for ( ; b < bits; ++b, n = mcc_bit_op_inc(n), v = mcc_bit_op_inc(v) ) {
		if ( c ) {
			if ( *(n.seg) & n.bit )
				c = 0;
			*(n.seg) ^= n.bit;
		}
		if ( *(v.seg) & v.bit ) {
			*(n.seg) ^= n.bit;
			if ( *(n.seg) & n.bit )
				c = 1;
		}
	}
	if ( c ) {
		for ( ; b < nb; ++b, n = mcc_bit_op_inc(n) ) {
			*(n.seg) ^= n.bit;
			if ( !(*(n.seg) & n.bit ) ) {
				c = 0;
				break;
			}
		}
	}
	return c ? EOVERFLOW : EXIT_SUCCESS;
}


int mcc___vint_op_mul( struct mcc__vint num, struct mcc__vint val, struct mcc__vint tmp ) {
	int ret = mcc_vint_validate3( &num, &val, &tmp );
	mcc_bit_t v, s;
	mcc_vint_seg_t bits = 0;
	if ( ret != EXIT_SUCCESS ) return ret;
	s = mcc_bit_op_inc(mcc_bit_op_dec_for_bit( val.stop, val.zero ));
	(void)mcc__vint_op_xor( tmp, tmp );
	(void)mcc__vint_op_aor( tmp, num );
	(void)mcc__vint_op_xor( num, num );
	for ( v = val.zero; v.b < s.b; v = mcc_bit_op_inc(v) ) {
		if ( *(v.seg) & v.bit ) {
			(void)mcc___vint_op_shl( tmp, bits );
			if ( mcc__vint_op_add( num, tmp ) == EOVERFLOW )
				ret = EOVERFLOW;
			bits = 0;
		}
		++bits;
	}
	return ret;
}

int mcc__vint_op_mul( struct mcc__vint num, struct mcc__vint val ) {
	int ret = mcc_vint_validate2( &num, &val );
	struct mcc__vint tmp = {0};
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = mcc_vint_size_and_fill( &tmp, num );
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = mcc___vint_op_mul( num, val, tmp );
	(void)mcc_vint_size( &tmp, 0 );
	return ret;
}

int mcc___vint_op_div( struct mcc__vint num, struct mcc__vint val, struct mcc__vint rem ) {
	int ret = mcc_vint_validate3( &num, &val, &rem );
	struct mcc__vint seg = {0}, tmp = {0};
	mcc_bit_t quo;
	if ( ret != EXIT_SUCCESS ) return ret;
	(void)mcc__vint_op_xor( rem, rem );
	(void)mcc__vint_op_aor( rem, num );
	(void)mcc__vint_op_xor( num, num );
	if ( mcc__vint_is_nil( val ) ) return EXIT_SUCCESS;
	tmp = val;
	tmp.stop = mcc_bit_op_inc(mcc_bit_op_dec_for_bit( tmp.stop, tmp.zero ));
	seg = rem;
	seg.zero = seg.stop;
	quo = num.stop;
	while ( mcc__vint_is_gte( rem, tmp ) ) {
		seg.zero = mcc_bit_op_dec(seg.zero);
		quo = mcc_bit_op_dec( quo );
		if ( mcc__vint_is_gte( seg, tmp ) ) {
			if ( mcc__vint_op_sub( seg, tmp ) == EOVERFLOW )
				ret = EOVERFLOW;
			*(quo.seg) |= quo.bit;
		}
		/* Fallback for poorly programmed code */
		//if ( seg.zero.b == num.zero.b ) break;
	}
	return ret;
}
int mcc__vint_op_div( struct mcc__vint num, struct mcc__vint val ) {
	int ret = mcc_vint_validate2( &num, &val );
	struct mcc__vint rem = {0};
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = mcc_vint_size( &rem, num.size );
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = mcc___vint_op_div( num, val, rem );
	(void)mcc_vint_size( &rem, 0 );
	return ret;
}
#if 0
int mcc__vint_op_mod( struct mcc__vint num, struct mcc__vint val ) {
	int ret = mcc_vint_validate2( &num, &val );
	struct mcc__vint rem = {0};
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = mcc_vint_size( &rem, num.size );
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = mcc___vint_op_div( num, val, rem );
	(void)memcpy( num.zero.seg, rem.zero.seg, num.size );
	(void)mcc_vint_size( &rem, 0 );
	return ret;
}
#else
int mcc__vint_op_mod( struct mcc__vint num, struct mcc__vint val ) {
	int ret = mcc_vint_validate2( &num, &val );
	struct mcc__vint seg = {0}, tmp = {0};
	if ( ret != EXIT_SUCCESS ) return ret;
	if ( mcc__vint_is_nil( val ) ) return EXIT_SUCCESS;
	tmp = val;
	tmp.stop = mcc_bit_op_inc(mcc_bit_op_dec_for_bit( tmp.stop, tmp.zero ));
	seg = num;
	seg.zero = seg.stop;
	while ( mcc__vint_is_gte( num, tmp ) ) {
		seg.zero = mcc_bit_op_dec(seg.zero);
		if ( mcc__vint_is_gte( seg, tmp ) ) {
			if ( mcc__vint_op_sub( seg, tmp ) == EOVERFLOW )
				ret = EOVERFLOW;
		}
		/* Fallback for poorly programmed code */
		//if ( seg.zero.b == num.zero.b ) break;
	}
	return ret;
}
#endif

int mcc__vint_op_shl( struct mcc__vint num, struct mcc__vint val ) {
	int ret = mcc_vint_validate2( &num, &val );
	struct mcc__vint tmp = {0}, cpy = {0};
	mcc_vint_seg_t move = 0, bits = 0;
	if ( ret != EXIT_SUCCESS ) return ret;
	bits = num.stop.b - num.zero.b;
	tmp = mcc_vint_wrap( 0, &bits, sizeof(bits) );
	if ( mcc__vint_is_gte( val, tmp ) ) {
		ret = mcc_vint_size_and_fill( &cpy, val );
		if ( ret != EXIT_SUCCESS ) return ret;
		ret = mcc__vint_op_mod( cpy, tmp );
		if ( ret != EXIT_SUCCESS ) return ret;
		move = *(cpy.zero.seg);
		(void)mcc_vint_size( &cpy, 0 );
	}
	else move = *(val.zero.seg);
	return mcc___vint_op_shl(num, move );
}
int mcc__vint_op_shr( struct mcc__vint num, struct mcc__vint val, bool neg ) {
	int ret = mcc_vint_validate2( &num, &val );
	struct mcc__vint tmp = {0}, cpy = {0};
	mcc_vint_seg_t move = 0, bits = 0;
	if ( ret != EXIT_SUCCESS ) return ret;
	bits = num.stop.b - num.zero.b;
	tmp = mcc_vint_wrap( 0, &bits, sizeof(bits) );
	move = *(val.zero.seg);
	if ( mcc__vint_is_gte( val, tmp ) ) {
		ret = mcc_vint_size_and_fill( &cpy, val );
		if ( ret != EXIT_SUCCESS ) return ret;
		ret = mcc__vint_op_mod( cpy, tmp );
		if ( ret != EXIT_SUCCESS ) return ret;
		move = *(cpy.zero.seg);
		(void)mcc_vint_size( &cpy, 0 );
	}
	else move = *(val.zero.seg);
	return mcc___vint_op_shr( num, move, neg );
}
