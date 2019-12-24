#include <mcc/core/vflt.h>
#include <ctype.h>
#include <pthread.h>
mcc_fpn_t mcc_fpn_set_inf( mcc_fpn_t mcc );
mcc_fpn_t mcc_fpn_make( mcc_fpn_t tmp, mcc_fpn_t ret );
mcc_fpn_t mcc_fpn_read(
	char *txt,
	mcc_fpn_t *gcc,
	mcc_fpn_t tmp,
	mcc_fpn_t ret );
mcc_big_t mcc_big_set_inf( mcc_big_t mcc );
mcc_big_t mcc_big_make( mcc_big_t tmp, mcc_big_t ret );
mcc_big_t mcc_big_read(
	char *txt,
	mcc_big_t *gcc,
	mcc_big_t tmp,
	mcc_big_t ret );
void printb( char const *pfx, void * addr, size_t bits, char const *sfx ) {
	mcc_bit_t b = {0};
	b.seg = addr;
	b.bit = 1;
	b = mcc__bit_op_add( b, bits );
	printf("%s",pfx);
	while ( b.b ) {
		b = mcc_bit_op_dec(b);
		putchar( '0' + !!(*(b.seg) & b.bit) );
	}
	printf("%s",sfx);
}
typedef struct waswasnot {
	uint was;
	uint not;
} waswasnot_t;
waswasnot_t waswasnot_add( waswasnot_t num, waswasnot_t val ) {
	num.was += val.was;
	num.not += val.not;
	return num;
}
void waswasnot_put( char *pfx, char *txt, waswasnot_t num, char *sfx ) {
	printf("%s was %s %u, ", pfx, txt, num.was );
	printf("%s not %s %u%s", pfx, txt, num.not, sfx );
}
typedef struct fpn_stats {
	uint out;
	ullong count;
	waswasnot_t eql;
	waswasnot_t mul;
	waswasnot_t nil;
	waswasnot_t inc;
	waswasnot_t shl;
	waswasnot_t inf;
} fpn_stats_t;
void fpn_stats_add( fpn_stats_t *dst, fpn_stats_t stats ) {
	dst->count += stats.count;
	dst->eql = waswasnot_add( dst->eql, stats.eql );
	dst->nil = waswasnot_add( dst->nil, stats.nil );
	dst->inc = waswasnot_add( dst->inc, stats.inc );
	dst->shl = waswasnot_add( dst->shl, stats.shl );
	dst->mul = waswasnot_add( dst->mul, stats.mul );
	dst->inf = waswasnot_add( dst->inf, stats.inf );
}
void fpn_stats_put( char *txt, char *pfx, fpn_stats_t stats ) {
	float right = stats.eql.was, total = stats.count;
	if ( !txt ) txt = "";
	if ( !pfx ) pfx = "";
	printf("%s() %s count %llu %s\n", __func__, pfx, stats.count, txt );
	waswasnot_put( pfx, "eql", stats.eql, "\n" );
	waswasnot_put( pfx, "nil", stats.nil, "\n" );
	waswasnot_put( pfx, "inc", stats.inc, "\n" );
	waswasnot_put( pfx, "shl", stats.shl, "\n" );
	waswasnot_put( pfx, "mul", stats.mul, "\n" );
	waswasnot_put( pfx, "inf", stats.inf, "\n" );
	printf( "%f%% Correct\n", (right / total) * 100.0 );
}
void details(
	fpn_stats_t *dst, char const *txt,
	mcc_fpn_t gcc, mcc_fpn_t mcc ) {
	FPN_UNION _gcc = {0}, _mcc = {0};
	ullong g = gcc.raw, m = mcc.raw;
	fpn_stats_t stats = {0};
	mcc_fpn_limits_t limits = mcc.limits;
	stats.count = 1;
	stats.eql.was = (gcc.raw == mcc.raw);
	stats.eql.not = !(stats.eql.was);
	if ( stats.eql.was ) goto details_add;
	_gcc.raw = gcc.raw;
	_mcc.raw = mcc.raw;
	_gcc.sig = 0;
	_mcc.sig = 0;
	stats.nil.was = (_gcc.raw && !(_mcc.raw));
	stats.nil.not = (!(_gcc.raw) && _mcc.raw);
	if ( stats.nil.was || stats.nil.not ) goto details_out;
	stats.inf.was = (_gcc.exp != mcc.inf && _mcc.exp >= mcc.inf);
	stats.inf.not = (_gcc.exp >= mcc.inf && _mcc.exp != mcc.inf);
	if ( stats.inf.was || stats.inf.not ) goto details_out;
	stats.inc.was = (gcc.raw == (mcc.raw - 1));
	stats.inc.not = (gcc.raw == (mcc.raw + 1));
	if ( stats.inc.was || stats.inc.not ) goto details_out;
	stats.mul.was = (_gcc.exp == (_mcc.exp - 1));
	stats.mul.not = (_gcc.exp == (_mcc.exp + 1));
	if ( stats.mul.was || stats.mul.not ) goto details_out;
	stats.shl.was = ((_gcc.man << 1) == _mcc.man);
	stats.shl.not = (!(stats.shl.was) && _gcc.man == (_mcc.man << 1));
	if ( stats.shl.was || stats.shl.not ) goto details_out;
	details_out:
	if ( !(dst->out) ) goto details_add;
	_gcc.raw = gcc.raw;
	_mcc.raw = mcc.raw;
	printf( "given %s\n", txt );
	printf( "expect " FPN_PRIe ", " FPN_PRIf "\n", _gcc.fpn, _gcc.fpn );
	printf( "result " FPN_PRIe ", " FPN_PRIf "\n", _mcc.fpn, _mcc.fpn );
	printb( "gcc = ", &(gcc.raw), bitsof(gcc.raw), "\n" );
	printb( "mcc = ", &(mcc.raw), bitsof(mcc.raw), "\n" );
	printb( "gcc mantissa = ", &g, limits.man_bits, "\n" );
	printb( "mcc mantissa = ", &m, limits.man_bits, "\n" );
	g >>= limits.man_bits;
	m >>= limits.man_bits;
	printb( "gcc exponent = ", &g, limits.exp_bits, "\n" );
	printb( "mcc exponent = ", &m, limits.exp_bits, "\n" );
	g >>= limits.exp_bits;
	m >>= limits.exp_bits;
	printb( "gcc negative = ", &g, 1, "\n" );
	printb( "mcc negative = ", &m, 1, "\n" );
	printf( "tmp.base = %llu\n", (ullong)mcc.base );
	printf( "tmp.neg = %llu\n", (ullong)(mcc.neg) );
	printf( "tmp.num = %llu\n", (ullong)(mcc.num) );
	printf( "tmp.fpn = %llu\n", (ullong)(mcc.fpn) );
	printf( "tmp.one = %llu\n", (ullong)(mcc.one) );
	printf( "tmp.tmp = %llu\n", (ullong)(mcc.tmp) );
	printf( "tmp.pos = %ld\n", mcc.pos );
	printf( "tmp.exp = %ld\n", mcc.exp );
	printf( "gcc.pos = %ld\n", gcc.pos );
	printf( "gcc.exp = %ld\n", gcc.exp );
	details_add:
	fpn_stats_add( dst, stats );
}
mcc_big_t fill_big( mcc_fpn_t mcc,
	mcc_big_t big, uchar *base, size_t size ) {
	big.limits = mcc.limits;
	big.rounds = mcc.rounds;
	big.epsilon = mcc.epsilon;
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
typedef struct fpn_thread {
	int ret;
	int i, c, f, e;
	size_t size;
	uchar *base;
	fpn_stats_t fast, slow;
	mcc_fpn_t nil;
	pthread_t inst;
	pthread_attr_t attr;
	pthread_mutex_t *mutex;
} fpn_thread_t;
#define CHECK_FAST 0
#define CHECK_SLOW 1
#define NO_THREAD 0
void* fpn_thread( fpn_thread_t *obj ) {
	mcc_fpn_t gcc = {0}, mcc = obj->nil, tmp;
	mcc_big_t big_gcc = {0}, big_mcc = {0}, big_tmp = {0};
	char txt[bitsof(double)] = {0};
	int i = obj->i, j, k, c = obj->c, f = obj->f, e = obj->e;
	fpn_stats_t fast = {0}, slow = {0};
	//fast.out = 1;
	big_gcc = fill_big( mcc, big_gcc, obj->base, obj->size );
	big_mcc = fill_big( mcc, big_mcc, &(obj->base[obj->size * 8]), obj->size );
	big_tmp = fill_big( mcc, big_tmp, &(obj->base[obj->size * 16]), obj->size );
	for ( ; i <= e; ++i ) {
		for ( j = 0; j <= c; ++j ) {
			for ( k = 0; k < f; ++k ) {
				memset( txt, 0, bitsof(double) );
				snprintf( txt, bitsof(double), "%d.%de%d", j, k, i );
#if CHECK_FAST
				tmp = mcc_fpn_read( txt, &gcc, mcc, mcc );
				details( &fast, txt, gcc, tmp );
#endif
#if CHECK_SLOW
				mcc_big_read( txt, &big_gcc, big_mcc, big_tmp );
				gcc.raw = *((mcc_uhuge_t*)(big_gcc.raw.zero.seg));
				tmp.raw = *((mcc_uhuge_t*)(big_mcc.raw.zero.seg));
				details( &slow, txt, gcc, tmp );
#endif	
				memset( txt, 0, bitsof(double) );
				snprintf( txt, bitsof(double), "%d.%de%d", j, k, -i );
#if CHECK_FAST
				tmp = mcc_fpn_read( txt, &gcc, mcc, mcc );
				details( &fast, txt, gcc, tmp );
#endif
#if CHECK_SLOW
				mcc_big_read( txt, &big_gcc, big_mcc, big_tmp );
				gcc.raw = *((mcc_uhuge_t*)(big_gcc.raw.zero.seg));
				tmp.raw = *((mcc_uhuge_t*)(big_mcc.raw.zero.seg));
				details( &slow, txt, gcc, tmp );
#endif
			}
		}
	}
#if !NO_THREAD
	pthread_mutex_lock( obj->mutex );
#endif
	obj->ret = 0;
	fpn_stats_add( &(obj->fast), fast );
	fpn_stats_add( &(obj->slow), slow );
#if !NO_THREAD
	pthread_mutex_unlock( obj->mutex );
#endif
	return obj;
}
int main() {
	int i, c = 50, f = c, set = c / 4;
	fpn_thread_t threads[4], *thread;
	pthread_t *inst;
	pthread_attr_t *attr;
	pthread_mutex_t mutex = {0};
	(void)memset( threads, 0, sizeof(fpn_thread_t) * 4 );
	mcc_fpn_limits_t limits = {0};
	fpn_stats_t fast = {0}, slow = {0};
	size_t size = 0;
	uchar *base = NULL;
	limits.exp_bias = FPN_MAX_EXP;
	limits.exp_bits = FPN_EXP_BIT;
	limits.max_exp_digits = FPN_MAX_10_EXP;
	limits.min_exp_digits = FPN_MIN_10_EXP;
	limits.max_exp = FPN_MAX_EXP;
	limits.min_exp = FPN_MIN_EXP;
	limits.man_bits = FPN_MAN_BIT;
	printf("Numbers limited to: %d.%de+/-%d\n", c, f, c );
	printf( "FLT_ROUNDS = %d\n", FLT_ROUNDS );
	printf( "FLT_EPSILON = %lf\n", FLT_EPSILON );
#if 0
	printf( "limits.exp_bias = %ld\n", limits.exp_bias );
	printf( "limits.exp_bits = %ld\n", limits.exp_bits );
	printf( "limits.max_exp_digits = %ld\n", limits.max_exp_digits );
	printf( "limits.min_exp_digits = %ld\n", limits.min_exp_digits );
	printf( "limits.max_exp = %ld\n", limits.max_exp );
	printf( "limits.min_exp = %ld\n", limits.min_exp );
	printf( "limits.man_bits = %ld\n", limits.man_bits );
#endif
	size = (limits.max_exp / CHAR_BIT);
	base = malloc( limits.max_exp * 8 );
	if ( !base ) return EXIT_FAILURE;
	for ( i = 0; i < 4; ++i ) {
		thread = &(threads[i]);
		inst = &(thread->inst);
		attr = &(thread->attr);
		/* Optimized memory allocations */
		thread->size = size;
		thread->base = &(base[limits.max_exp * i]);
		/* Default FPN data */
		thread->i = i * set;
		thread->c = c;
		thread->f = f;
		thread->e = thread->i + set;
		thread->nil.one = 1;
		thread->nil.base = 10;
		thread->nil.limits = limits;
		thread->nil.rounds = FLT_ROUNDS;
		thread->nil.epsilon = FLT_EPSILON;
		thread->nil = mcc_fpn_set_inf( thread->nil );
#if NO_THREAD
		fpn_thread( thread );
		fpn_stats_add( &fast, thread->fast );
		fpn_stats_add( &slow, thread->slow );
#else
		/* Initialise the thread */
		thread->mutex = &mutex;
		pthread_attr_init(attr);
		pthread_create( inst, NULL,
			(void* (*)(void*))fpn_thread, thread );
#endif
	}
#if !NO_THREAD
	for ( i = 0; i < 4; ++i ) {
		thread = &(threads[i]);
		pthread_join( thread->inst, NULL );
		fpn_stats_add( &fast, thread->fast );
		fpn_stats_add( &slow, thread->slow );
	}
#endif
	fpn_stats_put( "Fast set", NULL, fast );
	fpn_stats_put( "Slow set", NULL, slow );
	free(base);
	return EXIT_SUCCESS;
}
mcc_fpn_t mcc_fpn_set_inf( mcc_fpn_t mcc ) {
	mcc.inf = 0;
	mcc.inf = ~(mcc.inf);
	mcc.inf <<= mcc.limits.exp_bits;
	mcc.inf = ~(mcc.inf);
	return mcc;
}
mcc_fpn_t mcc_fpn_make( mcc_fpn_t mcc, mcc_fpn_t tmp ) {
	mcc_fpn_limits_t limits = mcc.limits;
	mcc.raw = 0;
	tmp = mcc;
	if ( tmp.pos > 0 || !(mcc.num) || !(mcc.one) ) goto mcc_fpn_sig;
	if ( !(tmp.num % tmp.one) ) {
		tmp.num /= tmp.one;
		tmp.one = 1;
		tmp.pos = 0;
	}
	while ( tmp.exp > 0 ) {
		if ( tmp.pos == 0 || tmp.one == 1 ) break;
		tmp.one /= tmp.base;
		tmp.pos++;
		tmp.exp--;
	}
	if ( tmp.exp < limits.min_exp_digits ) goto mcc_fpn_sig;
	if ( tmp.exp > limits.max_exp_digits ) {
		mcc_fpn_inf:
		tmp.raw = mcc.inf;
		goto mcc_fpn_exp;
	}
	for ( ; tmp.exp > 0; tmp.exp-- ) {
		tmp.tmp = tmp.num;
		tmp.num *= mcc.base;
		if ( tmp.num <= tmp.tmp ) goto mcc_fpn_inf;
	}
	for ( ; tmp.exp < 0; tmp.exp++ ) {
		tmp.tmp = tmp.one;
		tmp.one *= mcc.base;
		if ( tmp.one <= tmp.tmp ) goto mcc_fpn_sig;
	}
	if ( !(tmp.one) ) goto mcc_fpn_sig;
	tmp.fpn = tmp.num % tmp.one;
	tmp.num /= tmp.one;
	if ( !(tmp.fpn) ) tmp.one = 1;
	tmp.pos = 0;
	if ( tmp.num ) {
		for ( tmp.tmp = tmp.num;
			tmp.tmp > 1; tmp.pos++, tmp.tmp >>= 1 );
	}
	else {
		for ( tmp.tmp = tmp.one
			; tmp.tmp > tmp.fpn; tmp.pos--, tmp.tmp >>= 1 );
		if ( tmp.tmp == tmp.fpn && tmp.pos < -1 ) tmp.pos--;
	}
	mcc.pos = tmp.pos;
	tmp.raw = limits.exp_bias + (tmp.pos - 1);
	if ( tmp.raw > mcc.inf ) goto mcc_fpn_inf;
	if ( tmp.pos > limits.man_bits ) {
		tmp.pos -= limits.man_bits;
		tmp.fpn = 0;
		tmp.fpn = ~(tmp.fpn);
		tmp.fpn <<= tmp.pos;
		tmp.fpn = ~(tmp.fpn);
		tmp.fpn &= tmp.num;
		tmp.one <<= tmp.pos;
		mcc.raw = tmp.num >> tmp.pos;
		tmp.tmp = tmp.one / 2;
		if ( tmp.fpn > tmp.tmp ) mcc.raw++;
		else if ( tmp.fpn == tmp.tmp ) {
			switch ( mcc.rounds ) {
			case 1: if ( mcc.raw & 1u ) mcc.raw++; break;
			}
		}
	}
	else {
		mcc.raw = tmp.num;
		tmp.tmp = tmp.fpn;
#if 0
		if ( tmp.pos < -8 ) {
			tmp.tmp <<= -(tmp.pos + 8);
			mcc.raw <<= -(tmp.pos + 8);
			tmp.pos = -8;
		}
#endif
		for ( ; tmp.pos < limits.man_bits; tmp.pos++ ) {
			tmp.tmp *= 2;
			mcc.raw <<= 1;
			if ( tmp.tmp >= tmp.one ) {
				mcc.raw |= 1;
				tmp.tmp -= tmp.one;
			}
		}
		tmp.tmp *= 2;
		if ( tmp.tmp >= tmp.one ) mcc.raw++;
	}
	tmp.tmp = (bitsof(mcc.raw) - limits.man_bits);
	mcc.raw <<= tmp.tmp;
	mcc.raw >>= tmp.tmp;
	mcc_fpn_exp:
	tmp.raw <<= limits.man_bits;
	mcc.raw |= tmp.raw;
	mcc_fpn_sig:
	tmp.neg <<= (limits.man_bits + limits.exp_bits);
	mcc.raw |= tmp.neg;
	mcc.num = tmp.num;
	mcc.one = tmp.one;
	mcc.fpn = tmp.fpn;
	return mcc;
}
mcc_fpn_t mcc_fpn_read(
	char *txt,
	mcc_fpn_t *gcc,
	mcc_fpn_t tmp,
	mcc_fpn_t fpn
) {
	char *text, sig;
	int c = 0, l = 10, h = 10;
	char e = 'e', E = 'E';
	FPN_UNION lib = {0};
	while ( isspace(*txt) ) ++txt;
	text = txt;
	switch ( *txt ) {
		case '-': tmp.neg = 1;
		case '+': ++txt;
	}
	if ( *txt == '0' ) {
		++txt;
		switch ( *txt ) {
		case 'b': case 'B':
			++txt;
			tmp.base = 2;
			break;
		case 'x': case 'X':
			++txt;
			tmp.base = 16;
			break;
		}
		if ( tmp.base > 36 )
			l = 36;
	}
	while ( *txt ) {
		c = mcc_geti( *txt, l, h );
		if ( c < 0 || c >= tmp.base ) break;
		++txt;
		tmp.num *= tmp.base;
		tmp.num += c;
	}
	tmp.pos = 0;
	if ( *txt == '.' ) {
		++txt;
		while ( *txt ) {
			c = mcc_geti( *txt, l, h );
			if ( c < 0 || c >= tmp.base ) break;
			++txt;
			tmp.one *= tmp.base;
			tmp.num *= tmp.base;
			tmp.num += c;
			tmp.pos--;
		}
	}
	if ( *txt == e || *txt == E ) {
		++txt;
		switch ( *txt ) { case '-': case '+': sig = *txt; ++txt; }
		while ( *txt ) {
			c = mcc_geti( *txt, l, h );
			if ( c < 0 || c >= tmp.base ) break;
			++txt;
			tmp.exp *= tmp.base;
			tmp.exp += c;
		}
		if ( sig == '-' ) tmp.exp = -(tmp.exp);
	}
	*gcc = tmp;
	if ( tmp.base == 10 ) sscanf( text, FPN_SCNe, &(lib.fpn) );
	else if ( tmp.base == 16 ) sscanf( text, FPN_SCNa, &(lib.fpn) );
	gcc->raw = lib.raw;
	fpn = tmp;
	return mcc_fpn_make( fpn, tmp );
}
mcc_big_t mcc_big_set_inf( mcc_big_t big ) {
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
int mcc_big_validiate( mcc_big_t *mcc_big ) {
	int ret;
	if ( !mcc_big ) return EDESTADDRREQ;
	ret = mcc_vint_validate( &(mcc_big->base) );
	if ( ret != EXIT_SUCCESS ) return ret;
	return EXIT_SUCCESS;
}
mcc_big_t mcc_big_make( mcc_big_t mcc, mcc_big_t tmp ) {
	int ret = mcc_big_validiate(&mcc);
	mcc_vint_seg_t bits = 0;
	mcc_uhuge_t _one = 1, size, need;
	mcc_uvint_t one = {0};
	mcc_fpn_limits_t limits = mcc.limits;
	if ( ret != EXIT_SUCCESS ) {
		printf("Error: %08X, %s\n", ret, strerror(ret) );
		return mcc;
	}
	ret = mcc_big_validiate(&tmp);
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
		goto mcc_big_sig;
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
	if ( tmp.exp < limits.min_exp_digits ) goto mcc_big_sig;
	if ( tmp.exp > limits.max_exp_digits ) {
		mcc_big_inf:
		mcc_vint_fill( tmp.raw, mcc.inf );
		goto mcc_big_exp;
	}
	for ( ; tmp.exp > 0; tmp.exp-- ) {
		if ( mcc___vint_op_mul( tmp.num, tmp.base, tmp.tmp ) ==
			EOVERFLOW ) goto mcc_big_inf;
	}
	for ( ; tmp.exp < 0; tmp.exp++ ) {
		if ( mcc___vint_op_mul( tmp.one, tmp.base, tmp.tmp ) ==
			EOVERFLOW ) goto mcc_big_sig;
	}
	if ( mcc__vint_is_nil( tmp.one ) ) goto mcc_big_sig;
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
	if ( mcc__vint_is_gth( tmp.raw, mcc.inf ) ) goto mcc_big_inf;
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
	mcc_big_exp:
	mcc___vint_op_shl( tmp.raw, limits.exp_bits );
	mcc__vint_op_aor( mcc.raw, tmp.raw );
	mcc_big_sig:
	mcc___vint_op_shl( tmp.neg, limits.man_bits + limits.exp_bits );
	return mcc;
}
mcc_big_t mcc_big_read(
	char *txt,
	mcc_big_t *gcc,
	mcc_big_t tmp,
	mcc_big_t big
) {
	char *text, sig;
	int c = 0, l = 10, h = 10;
	char e = 'e', E = 'E';
	FPN_UNION lib = {0};
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
	*gcc = tmp;
	mcc_vint_fill( gcc->inf, tmp.inf );
	mcc_vint_fill( gcc->num, tmp.num );
	mcc_vint_fill( gcc->fpn, tmp.fpn );
	mcc_vint_fill( gcc->one, tmp.one );
	mcc_vint_fill( C, tmp.base );
	if ( c == 10 ) sscanf( text, FPN_SCNe, &(lib.fpn) );
	else if ( c == 16 ) sscanf( text, FPN_SCNa, &(lib.fpn) );
	mcc_vint_to_val( gcc->raw, lib.raw );
	mcc_vint_fill( big.inf, tmp.inf );
	mcc_vint_fill( big.num, tmp.num );
	mcc_vint_fill( big.fpn, tmp.fpn );
	mcc_vint_fill( big.one, tmp.one );
	return mcc_big_make( big, tmp );
}
