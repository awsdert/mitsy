#include "mcc_base62.h"

int ubase62(
	MCC_GETS *src, mcc_utf_t utf,
	ubase62_t *dst, int base, bool lowislow, long min_len, long max_len )
{
	int ret = mcc_gets_test(src), c;
	long i = 0, l, h, len = 0, dec = -1;
	ubase62_t num = 0;
	if ( ret != EXIT_SUCCESS )
		goto ubase62_done;
	if ( min_len < 0 ) min_len = 0;
	if ( max_len < 1 ) max_len = LONG_MAX - 1;
	else if ( base < 2 ) {
		ret = ERANGE;
		FAIL( stderr, ret, "base is too low!" );
		goto ubase62_done;
	}
	else if ( base <= 36 ) { l = 10; h = 10; }
	else if ( base > 62 ) {
		ret = ERANGE;
		FAIL( stderr, ret, "base is too high!" );
		goto ubase62_done;
	}
	else if ( lowislow ) { l = 10; h = 36; }
	else { l = 36; h = 10; }
	do {
		c = utf[0];
		if ( c >= U'0' && c <= U'9' )
			c -= U'0';
		else if ( c >= U'A' && c <= U'Z' )
			{ c -= U'A'; c += h; }
		else if ( c >= U'a' && c <= U'z' )
			{ c -= U'a'; c += l; }
		else break;
		if ( c >= base || i >= max_len ) break;
		num *= base;
		num += c;
		++i;
	} while ( (ret = mcc_getc(src,utf,&len)) == EXIT_SUCCESS );
	if ( c == U'.' ) {
		dec = 0;
		do {
			c = utf[0];
			if ( c >= U'0' && c <= U'9' )
				c -= U'0';
			else if ( c >= U'A' && c <= U'Z' )
				{ c -= U'A'; c += h; }
			else if ( c >= U'a' && c <= U'z' )
				{ c -= U'a'; c += l; }
			else break;
			if ( c >= base || i >= max_len ) break;
			num *= base;
			num += c;
			++i; ++dec;
		} while ( (ret = mcc_getc(src,utf,&len)) == EXIT_SUCCESS );
	}
	switch ( ret ) {
	case EXIT_SUCCESS: case ENODATA: break;
	default:
		FAIL( stderr, ret, "" );
	}
	if ( i < min_len ) {
		ret = ERANGE;
		FAIL( stderr, ret, "Not enough digits" );
		fprintf( stderr,
			"c = '%s', i = %ld, min_len = %ld, max_len = %ld\n",
			utf, i, min_len, max_len );
	}
	ubase62_done:
	if ( dst ) *dst = num;
	return ret;
}
