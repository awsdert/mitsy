#include "vflt.h"
char *test_vflt_literals[] = {
	"0",
	"1", "10", "16", "100", "101",
	"0.1", "0.01", "0.001", "0.101",
	"1.1", "1.01", "1.001", "1.101", "3.14",
	"1e+1", "1e+8", "1e+10", "1e+100", "3e+1",
	"1e-1", "1e-8", "1e-10", "1e-100", "3e-1",
	".1e+1", ".1e+8", ".1e+10", ".1e+100", ".3e+1",
	".1e-1", ".1e-8", ".1e-10", ".1e-100", ".3e-1",
	"1.1e+1", "1.1e+8", "1.1e+10", "1.1e+100", "3.1e+1",
	"1.1e-1", "1.1e-8", "1.1e-10", "1.1e-100", "3.1e-1",
	"3.14e+1", "3.14e+8", "3.14e+10", "3.14e+100",
	"3.14e-1", "3.14e-8", "3.14e-10", "3.14e-100",
	"-0",
	"-1", "-10", "-16", "-100", "-101",
	"-0.1", "-0.01", "-0.001", "-0.101",
	"-1.1", "-1.01", "-1.001", "-1.101", "-3.14",
	"-1e+1", "-1e+8", "-1e+10", "-1e+100", "-3e+1",
	"-1e-1", "-1e-8", "-1e-10", "-1e-100", "-3e-1",
	"-0.1e+1", "-0.1e+8", "-0.1e+10", "-0.1e+100", "-0.3e+1",
	"-0.1e-1", "-0.1e-8", "-0.1e-10", "-0.1e-100", "-0.3e-1",
	"-1.1e+1", "-1.1e+8", "-1.1e+10", "-1.1e+100", "-3.1e+1",
	"-1.1e-1", "-1.1e-8", "-1.1e-10", "-1.1e-100", "-3.1e-1",
	"-3.14e+1", "-3.14e+8", "-3.14e+10", "-3.14e+100",
	"-3.14e-1", "-3.14e-8", "-3.14e-10", "-3.14e-100",
	"0xA", "0xA0", "0xA6", "0xA00", "0xA0A",
	"0x0.A", "0x0.0A", "0x0.00A", "0x0.A0A",
	"0xA.A", "0xA.0A", "0xA.00A", "0xA.A0A", "0xC.A4",
	"0xAp+A", "0xAp+F", "0xAp+A0", "0xAp+A00", "0xCp+A",
	"0xAp-A", "0xAp-F", "0xAp-A0", "0xAp-A00", "0xCp-A",
	"0x0.Ap+A", "0x0.Ap+F", "0x0.Ap+A0", "0x0.Ap+A00", "0x0.Cp+A",
	"0x0.Ap-A", "0x0.Ap-F", "0x0.Ap-A0", "0x0.Ap-A00", "0x0.Cp-A",
	"0xA.Ap+A", "0xA.Ap+F", "0xA.Ap+A0", "0xA.Ap+A00", "0xC.Ap+A",
	"0xA.Ap-A", "0xA.Ap-F", "0xA.Ap-A0", "0xA.Ap-A00", "0xC.Ap-A",
	"0xC.A4p+A", "0xC.A4p+F", "0xC.A4p+A0", "0xC.A4p+A00",
	"0xC.A4p-A", "0xC.A4p-F", "0xC.A4p-A0", "0xC.A4p-A00",
	"-0xA", "-0xA0", "-0x10", "-0xA00", "-0xA0A",
	"-0x0.A", "-0x0.0A", "-0x0.00A", "-0x0.A0A",
	"-0xA.A", "-0xA.0A", "-0xA.00A", "-0xA.A0A", "-0xC.A4",
	"-0xAp+A", "-0xAp+F", "-0xAp+A0", "-0xAp+A00", "-0xCp+A",
	"-0xAp-A", "-0xAp-F", "-0xAp-A0", "-0xAp-A00", "-0xCp-A",
	"-0x0.Ap+A", "-0x0.Ap+F", "-0x0.Ap+A0", "-0x0.Ap+A00", "-0x0.Cp+A",
	"-0x0.Ap-A", "-0x0.Ap-F", "-0x0.Ap-A0", "-0x0.Ap-A00", "-0x0.Cp-A",
	"-0xA.Ap+A", "-0xA.Ap+F", "-0xA.Ap+A0", "-0xA.Ap+A00", "-0xC.Ap+A",
	"-0xA.Ap-A", "-0xA.Ap-F", "-0xA.Ap-A0", "-0xA.Ap-A00", "-0xC.Ap-A",
	"-0xC.A4p+A", "-0xC.A4p+F", "-0xC.A4p+A0", "-0xC.A4p+A00",
	"-0xC.A4p-A", "-0xC.A4p-F", "-0xC.A4p-A0", "-0xC.A4p-A00",
	NULL
};
int test__vflt_literal( mcc_mem_t mem,
	mcc_vflt_t *mcc, mcc_vflt_t *tmp, char * text );
void test__vflt_fill_vint(
	void *addr, size_t size, struct mcc__vint *num ) {
	*num = mcc_vint_wrap( 0, addr, size );
}
int test_vflt() {
	mcc_mem_t mem = {0};
	mcc_vflt_t mcc = {0}, tmp = {0};
	size_t total = 2048, per_num = total / 16, byte = 0;
	uchar *buff = NULL;
	/* Ensure we have enough memory for even long double testing */
	int ret = mcc_memsize(&mem,total), i;
	if ( ret != EXIT_SUCCESS ) return ret;
	buff = mem.addr;
	test__vflt_fill_vint( buff, per_num, &(mcc.inf) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(mcc.base) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(mcc.neg) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(mcc.num) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(mcc.fpn) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(mcc.one) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(mcc.tmp) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(mcc.raw) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(tmp.inf) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(tmp.base) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(tmp.neg) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(tmp.num) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(tmp.fpn) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(tmp.one) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(tmp.tmp) );
	byte += per_num;
	test__vflt_fill_vint( &(buff[byte]), per_num, &(tmp.raw) );
	puts("Starting vflt unit tests...");
	for ( i = 0; test_vflt_literals[i]; ++i ) {
		ret = test__vflt_literal( mem, &mcc, &tmp, test_vflt_literals[i] );
		if ( ret != EXIT_SUCCESS ) break;
	}
	mcc_memsize( &mem, 0 );
	return ret;
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
	mcc_vflt_t gcc, mcc_vflt_t mcc ) {
	ullong g = 0, m = 0;
	fpn_stats_t stats = {0};
	mcc_fpn_limits_t limits = mcc.limits;
	stats.count = 1;
	stats.eql.was = mcc__vint_is_eql( gcc.raw, mcc.raw );
	stats.eql.not = !(stats.eql.was);
	if ( stats.eql.was ) goto details_add;
#if 0
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
#endif
	if ( !(dst->out) ) goto details_add;
	memcpy( &g, gcc.raw.zero.seg, sizeof(ullong) );
	memcpy( &m, mcc.raw.zero.seg, sizeof(ullong) );
	printf( "given %s\n", txt );
#if 0
	_gcc.raw = gcc.raw;
	_mcc.raw = mcc.raw;
	printf( "expect " FPN_PRIe ", " FPN_PRIf "\n", _gcc.fpn, _gcc.fpn );
	printf( "result " FPN_PRIe ", " FPN_PRIf "\n", _mcc.fpn, _mcc.fpn );
#endif
	mcc_printb( "gcc = ", &(gcc.raw), bitsof(gcc.raw), "\n" );
	mcc_printb( "mcc = ", &(mcc.raw), bitsof(mcc.raw), "\n" );
	mcc_printb( "gcc mantissa = ", &g, limits.man_bits, "\n" );
	mcc_printb( "mcc mantissa = ", &m, limits.man_bits, "\n" );
	g >>= limits.man_bits;
	m >>= limits.man_bits;
	mcc_printb( "gcc exponent = ", &g, limits.exp_bits, "\n" );
	mcc_printb( "mcc exponent = ", &m, limits.exp_bits, "\n" );
	g >>= limits.exp_bits;
	m >>= limits.exp_bits;
	mcc_printb( "gcc negative = ", &g, 1, "\n" );
	mcc_printb( "mcc negative = ", &m, 1, "\n" );
	memcpy( &m, mcc.base.zero.seg, sizeof(ullong) );
	printf( "tmp.base = %llu\n", m );
	memcpy( &m, mcc.neg.zero.seg, sizeof(ullong) );
	printf( "tmp.neg = %llu\n", m );
	memcpy( &m, mcc.num.zero.seg, sizeof(ullong) );
	printf( "tmp.num = %llu\n", m );
	memcpy( &m, mcc.fpn.zero.seg, sizeof(ullong) );
	printf( "tmp.fpn = %llu\n", m );
	memcpy( &m, mcc.one.zero.seg, sizeof(ullong) );
	printf( "tmp.one = %llu\n", m );
	memcpy( &m, mcc.tmp.zero.seg, sizeof(ullong) );
	printf( "tmp.tmp = %llu\n", m );
	printf( "tmp.pos = %ld\n", mcc.pos );
	printf( "tmp.exp = %ld\n", mcc.exp );
	printf( "gcc.pos = %ld\n", gcc.pos );
	printf( "gcc.exp = %ld\n", gcc.exp );
	details_add:
	fpn_stats_add( dst, stats );
}
int test__vflt_literal( mcc_mem_t mem,
	mcc_vflt_t *mcc, mcc_vflt_t *tmp, char * text )
{
	mcc_fpn_limits_t FLT_LIMITS = {0},
		DBL_LIMITS = {0}, LDBL_LIMITS = {0};
	mcc_flt_union_t gcc_flt_union = {0}, mcc_flt_union = {0};
	mcc_dbl_union_t gcc_dbl_union = {0}, mcc_dbl_union = {0};
	mcc_ldbl_union_t gcc_ldbl_union = {0}, mcc_ldbl_union = {0};
	FLT_LIMITS.max_exp = FLT_MAX_EXP;
	FLT_LIMITS.max_exp_value = FLT_MAX_10_EXP;
	FLT_LIMITS.min_exp = FLT_MIN_EXP;
	FLT_LIMITS.min_exp_value = FLT_MIN_10_EXP;
	FLT_LIMITS.man_bits = FLT_MAN_BIT;
	FLT_LIMITS.exp_bits = FLT_EXP_BIT;
	FLT_LIMITS.exp_bias = FLT_MAX_EXP;
	mcc->limits = FLT_LIMITS;
	tmp->limits = FLT_LIMITS;
	memset( mem.addr, 0, mem.size );
	*mcc = mcc_vflt_literal( text, *tmp, *mcc );
	DBL_LIMITS.max_exp = DBL_MAX_EXP;
	DBL_LIMITS.max_exp_value = DBL_MAX_10_EXP;
	DBL_LIMITS.min_exp = DBL_MIN_EXP;
	DBL_LIMITS.min_exp_value = DBL_MIN_10_EXP;
	DBL_LIMITS.man_bits = DBL_MAN_BIT;
	DBL_LIMITS.exp_bits = DBL_EXP_BIT;
	DBL_LIMITS.exp_bias = DBL_MAX_EXP;
	mcc->limits = DBL_LIMITS;
	tmp->limits = DBL_LIMITS;
	memset( mem.addr, 0, mem.size );
	*mcc = mcc_vflt_literal( text, *tmp, *mcc );
	LDBL_LIMITS.max_exp = LDBL_MAX_EXP;
	LDBL_LIMITS.max_exp_value = LDBL_MAX_10_EXP;
	LDBL_LIMITS.min_exp = LDBL_MIN_EXP;
	LDBL_LIMITS.min_exp_value = LDBL_MIN_10_EXP;
	LDBL_LIMITS.man_bits = LDBL_MAN_BIT;
	LDBL_LIMITS.exp_bits = LDBL_EXP_BIT;
	LDBL_LIMITS.exp_bias = LDBL_MAX_EXP;
	mcc->limits = LDBL_LIMITS;
	tmp->limits = LDBL_LIMITS;
	memset( mem.addr, 0, mem.size );
	*mcc = mcc_vflt_literal( text, *tmp, *mcc );
	return EXIT_SUCCESS;
}
