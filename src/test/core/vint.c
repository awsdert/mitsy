#include "../../mcc/core/vint.c"
int test__vint( mcc_test_t num, char *op, mcc_test_t val );

char *operations[] = {
	"==", "!=", "!", ">", ">=", "<", "<=",
	"~", "<<", ">>", "|", "&", "^",
	"++", "+", "*", "--", "-", "/", "%",
NULL };

typedef time_t mcc_rnd_t;
long mcc__rnd( mcc_rnd_t *ctx, long min, long max ) {
	mcc_rnd_t bit, seed = time(NULL), mov = 0;
	long val;
	if ( ctx ) mov = *ctx;
	bit <<= mov++;
	if ( !bit || bit > seed ) {
		bit = 1;
		mov = 0;
	}
	val = (seed & bit) ? 1 : 0;
	bit <<= 1;
	if ( ctx ) *ctx = mov;
	if ( min > max ) min = max;
	if ( min != max ) {
		seed *= clock();
		bit = ~((~0u) << mov);
		val = (seed & bit);
		if ( val > max ) val = max;
		val -= (min >= 0) ? min : -min;
		return ( val < min ) ? min : val;
	}
	return val ? max : min;
}
#define mcc_rnd( ctx ) mcc__rnd( ctx, LONG_MIN, LONG_MAX )

int test_vint() {
	mcc_test_t i, j;
	mcc_rnd_t ctx;
	for ( j = 0; j < 10; ++j ) {
		for ( i = 0; operations[i]; ++i ) {
			test__vint( mcc_rnd(&ctx), operations[i], mcc_rnd(&ctx) );
		}
	}
	return 0;
}

int test__vint( mcc_test_t num, char *op, mcc_test_t val ) {
	int ret = EXIT_FAILURE;
	mcc_test_t rem = num, b4 = num;
	struct mcc__vint _num = {0}, _val = {0}, _rem = {0};
	(void)mcc_vint_size_and_fill( &_num, &num, sizeof(num) );
	(void)mcc_vint_size_and_fill( &_val, &val, sizeof(num) );
	(void)mcc_vint_size_and_fill( &_rem, &num, sizeof(num) );
	switch ( *op ) {
		case 0: ret = EILSEQ; goto fail;
		case '~': num = ~num; mcc__vint_op_not( &_num ); goto done;
		case '|': num |= val; mcc__vint_op_aor( &_num, &_val ); goto done;
		case '^': num ^= val; mcc__vint_op_xor( &_num, &_val ); goto done;
		case '&': num &= val; mcc__vint_op_and( &_num, &_val ); goto done;
		case '+':
			if ( op[1] == '+' )
				{ num++; mcc__vint_op_inc( &_num ); }
			else
				{ num += val; mcc__vint_op_add( &_num, &_val ); }
			goto done;
		case '*': num *= val; mcc__vint_op_mul( &_num, &_val ); goto done;
		case '-':
			if ( op[1] == '+' )
				{ num--; mcc__vint_op_dec( &_num ); }
			else
				{ num -= val; mcc__vint_op_sub( &_num, &_val ); }
			goto done;
		case '/':
			if ( val ) { rem %= val; num /= val; }
			else { rem = num; num = 0; }
			mcc___vint_op_div( &_num, &_val, &_rem );
			goto done;
		case '%':
			if ( val ) { rem %= val; num = rem; }
			else { rem = num; }
			mcc___vint_op_div( &_num, &_val, &_rem );
			memcpy( _num.zero.seg, _rem.zero.seg, _num.size );
			goto done;
		case '=':
			if ( op[1] != '=' ) goto fail;
			num = (num == val);
			struct mcc__vinto_num( &_num, mcc__vint_is_eql( &_num, &_val ) );
			goto done;
		case '!':
			if ( op[1] == '\0' ) {
				num = !num;
				struct mcc__vinto_num( &_num, mcc__vint_is_nil( &_num ) );
			}
			else if ( op[1] == '=' ) {
				num = (num != val);
				struct mcc__vinto_num( &_num, mcc__vint_is_neq( &_num, &_val ) );
			}
			goto done;
		case '>':
			if ( op[1] == '>' ) {
				num >>= val;
				mcc__vint_op_shr( &_num, &_val );
			}
			else if ( op[1] == '=' ) {
				num = (num >= val);
				struct mcc__vinto_num( &_num, mcc__vint_is_gte( &_num, &_val ) );
			}
			else {
				num = (num > val);
				struct mcc__vinto_num( &_num, mcc__vint_is_gth( &_num, &_val ) );
			}
			goto done;
		case '<':
			if ( op[1] == '<' ) {
				num <<= val;
				mcc__vint_op_shl( &_num, &_val );
			}
			else if ( op[1] == '=' ) {
				num = (num <= val);
				struct mcc__vinto_num( &_num, mcc__vint_is_lte( &_num, &_val ) );
			}
			else {
				num = (num < val);
				struct mcc__vinto_num( &_num, mcc__vint_is_lth( &_num, &_val ) );
			}
			goto done;
	}
	done:
	if ( *((mcc_test_t*)(_num.zero.seg)) == num )
		ret = EXIT_SUCCESS;
	else
		printf( "_num = %08" MCC_TEST_PRI_PFX "X, "
			"num = %08" MCC_TEST_PRI_PFX "X, "
			"b4 = %08" MCC_TEST_PRI_PFX "X, "
			"val = %" MCC_TEST_PRI_PFX "u, "
			"_rem = %08" MCC_TEST_PRI_PFX "X, "
			"rem = %08" MCC_TEST_PRI_PFX "X "
			"op = '%s'\n",
			*((mcc_test_t*)(_num.zero.seg)), num, b4, val,
			*((mcc_test_t*)(_rem.zero.seg)), rem, op );
	fail:
	mcc_vint_size( &_num, 0 );
	mcc_vint_size( &_val, 0 );
	mcc_vint_size( &_rem, 0 );
	return ret;
}
