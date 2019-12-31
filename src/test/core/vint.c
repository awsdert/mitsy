#include "vint.h"
int test__vint( mcc_test_t num, char *op, mcc_test_t val );

char *test_vint_operations[] = {
	"==", "!=", "!", ">", ">=", "<", "<=",
	"~", "<<", ">>", "|", "&", "^",
	"++", "+", "*", "--", "-", "/", "%",
NULL };

int test_vint() {
	mcc_test_t i, j;
	mcc_rnd_t ctx = 0;
	for ( j = 0; j < 3; ++j ) {
		for ( i = 0; test_vint_operations[i]; ++i ) {
			test__vint( mcc_rnd(&ctx), test_vint_operations[i], mcc_rnd(&ctx) );
		}
	}
	return 0;
}

int test__vint( mcc_test_t num, char *op, mcc_test_t val ) {
	int ret = EXIT_FAILURE;
	mcc_test_t rem = num, b4 = num;
	mcc_uvint_t _num = {0}, _val = {0}, _rem = {0};
	printf("Testing vint operations for %"
		MCC_TEST_PRI_PFX "u %s %" MCC_TEST_PRI_PFX "u... ",
		num, op, val );
	(void)mcc_vint_size_and_fill( &_num,
		mcc_vint_wrap( 0, &num, sizeof(num) ) );
	(void)mcc_vint_size_and_fill( &_val,
		mcc_vint_wrap( 0, &val, sizeof(num) ) );
	(void)mcc_vint_size_and_fill( &_rem,
		mcc_vint_wrap( 0, &num, sizeof(num) ) );
	switch ( *op ) {
		case 0: ret = EILSEQ; goto fail;
		case '~': num = ~num; mcc__vint_op_not( _num ); goto done;
		case '|': num |= val; mcc__vint_op_aor( _num, _val ); goto done;
		case '^': num ^= val; mcc__vint_op_xor( _num, _val ); goto done;
		case '&': num &= val; mcc__vint_op_and( _num, _val ); goto done;
		case '+':
			if ( op[1] == '+' )
				{ num++; mcc__vint_op_inc( _num ); }
			else
				{ num += val; mcc__vint_op_add( _num, _val ); }
			goto done;
		case '*': num *= val; mcc__vint_op_mul( _num, _val ); goto done;
		case '-':
			if ( op[1] == '+' )
				{ num--; mcc__vint_op_dec( _num ); }
			else
				{ num -= val; mcc__vint_op_sub( _num, _val ); }
			goto done;
		case '/':
			if ( val ) { rem %= val; num /= val; }
			else { rem = num; num = 0; }
			mcc___vint_op_div( _num, _val, _rem );
			goto done;
		case '%':
			if ( val ) { rem %= val; num = rem; }
			else { rem = num; }
			mcc___vint_op_div( _num, _val, _rem );
			memcpy( _num.zero.seg, _rem.zero.seg, _num.size );
			goto done;
		case '=':
			if ( op[1] != '=' ) goto fail;
			num = (num == val);
			mcc_vint_to_val( _num, mcc__vint_is_eql( _num, _val ) );
			goto done;
		case '!':
			if ( op[1] == '\0' ) {
				num = !num;
				mcc_vint_to_val( _num, mcc__vint_is_nil( _num ) );
			}
			else if ( op[1] == '=' ) {
				num = (num != val);
				mcc_vint_to_val( _num, mcc__vint_is_neq( _num, _val ) );
			}
			goto done;
		case '>':
			if ( op[1] == '>' ) {
				num >>= val;
				mcc__vint_op_shr( _num, _val, 0 );
			}
			else if ( op[1] == '=' ) {
				num = (num >= val);
				mcc_vint_to_val( _num, mcc__vint_is_gte( _num, _val ) );
			}
			else {
				num = (num > val);
				mcc_vint_to_val( _num, mcc__vint_is_gth( _num, _val ) );
			}
			goto done;
		case '<':
			if ( op[1] == '<' ) {
				num <<= val;
				mcc__vint_op_shl( _num, _val );
			}
			else if ( op[1] == '=' ) {
				num = (num <= val);
				mcc_vint_to_val( _num, mcc__vint_is_lte( _num, _val ) );
			}
			else {
				num = (num < val);
				mcc_vint_to_val( _num, mcc__vint_is_lth( _num, _val ) );
			}
			goto done;
	}
	done:
	if ( *((mcc_test_t*)(_num.zero.seg)) == num ) {
		puts("Results match!\n");
		ret = EXIT_SUCCESS;
	}
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
