#include "mcc_tsc.h"
#include "mcc_get.h"
#include <time.h>

#define BASE_NUM u8"0123456789"
#define BASE_a2z u8"abcdefghijklmnopqrstuvqxyz"
#define BASE_A2Z u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ"

char upper_base62_text[] = BASE_NUM BASE_A2Z BASE_a2z;
char lower_base62_text[] = BASE_NUM BASE_a2z BASE_A2Z;
int rdU64_base62( NEXTC *_nextc,
	uint base, _Bool lowercase1st, uint_least64_t *num ) {
	uint i = 0;
	uint_least64_t n = 0;
	int ret = nextc_validate( _nextc );
	if ( ret != 0 ) return ret;
	int rdU64_high() {
		char *base_text =
			lowercase1st ? lower_base62_text : upper_base62_text;
		for ( i = 0; i < base && _nextc->c[0] != base_text[i]; ++i );
		i = (i < base) ? i : 0;
		while ( i < base && nextc( _nextc ) ) {
			n *= base; n += i;
			for ( i = 0; i < base && _nextc->c[0] != base_text[i]; ++i );
		}
		if ( num ) *num = ((i < base) ? (n * base) + i : n);
		return 0;
	}
	int rdU64_both() {
		for ( i = 0; i < base &&
				_nextc->c[0] != upper_base62_text[i] &&
				_nextc->c[0] != lower_base62_text[i]; ++i );
		i = (i < base) ? i : 0;
		while ( i < base && nextc( _nextc ) ) {
			n *= base;
			n += i;
			for ( i = 0; i < base &&
				_nextc->c[0] != upper_base62_text[i] &&
				_nextc->c[0] != lower_base62_text[i]; ++i );
		}
		if ( num ) *num = ((i < base) ? (n * base) + i : n);
		return 0;
	}
	return (base < 2) ? -1 :
		((base < 37) ? rdU64_both() : ((base < 63) ? rdU64_high() : -1));
}
typedef struct num_out {
	uint unSigned : 1;
	uint isSigned : 1;
	uint use_base : 8;
	uint bitcount : 16;
	uint_least64_t num;
} num_out_t;
int rdNum( NEXTC *_nextc, num_out_t *out ) {
	int ret = nextc_validate(_nextc);
	num_out_t o = {0};
	o.use_base = 10;
	if ( ret != 0 ) return 0;
	void rdUint() {
		void cropH() {
			ret = 1;
			if ( !nextc( _nextc ) ) return;
			o.num = (_nextc->c == _nextc->p) ?
				(uchar)(o.num) : (ushort)(o.num);
			o.bitcount = (_nextc->c == _nextc->p) ? CHAR_BIT : bitsof(short);
			ret = 0;
		}
		void cropL() {
			ret = 1;
			if ( !nextc( _nextc ) ) return;
			o.num = (_nextc->c == _nextc->p) ?
				(ullong)(o.num) : (ulong)(o.num);
			o.bitcount = (_nextc->c == _nextc->p) ? bitsof(sllong) : bitsof(long);
			ret = 0;
		}
		void cropI() {
			uint_least64_t size = 0;
			uint_least64_t cropV() {
				uint_least64_t bit = -1;
				bit <<= size;
				o.num &= (~bit);
				ret = 0;
				return o.num;
			}
			if ( _nextc->c[0] == U'i' || _nextc->c[0] == U'I' ) {
				ret = 1;
				if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ) return;
				/* UTF-32 must be used here */
				if ( _nextc->c[0] >= U'0' && _nextc->c[0] <= U'9' ) {
					ret = rdU64_base62( _nextc, 10, 0, &size );
					if ( ret != 0 ) return;
				}
			}
			o.num = (size == 64) ? (o.num) :
				((size == 32) ? (uint_least32_t)(o.num) :
				((size == 16) ? (uint_least16_t)(o.num) :
				((size == 8) ? (uint_least8_t)(o.num) :
				(size ? cropV() : (uint)(o.num)))));
			o.bitcount = size ? size : bitsof(int);
			ret = 0;
		}
		ret = 1;
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ) return;
		o.use_base = (_nextc->c[0] == U'x' || _nextc->c[0] == U'X') ? 16 :
			((_nextc->c[0] == U'b' || _nextc->c[0] == U'B') ? 2 : 8);
		if (o.use_base != 8 && (ret = nextc(_nextc)) != EXIT_SUCCESS ) {
			ret = 1;
			return;
		}
		ret = rdU64_base62( _nextc, o.use_base, 0, &(o.num) );
		if ( _nextc->c[0] == U'u' || _nextc->c[0] == U'U' ) {
			if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ) {
				ret = 1;
				return;
			}
			o.unSigned = 1;
		}
		if (_nextc->c[0] == U'l' || _nextc->c[0] == U'L') cropL();
		else if (_nextc->c[0] == U'h' || _nextc->c[0] == U'H') cropH();
		else cropI();
	}
	void rdVal() {
		if (_nextc->c[0] == U'0') rdUint();
		else ret = rdU64_base62( _nextc, 10, 0, &(o.num) );
	}
	void rdInt() {
		ret = 1;
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ) return;
		o.isSigned = 1;
		rdVal();
		o.num = (_nextc->p[0] == U'-') ? -(o.num) : o.num;
	}
	if (_nextc->c[0] == U'-' || _nextc->c[0] == U'+') rdInt();
	else rdVal();
	if ( out ) *out = o;
	return ret;
}
int rdNum2( NEXTC *_nextc, num_out_t *out ) {
	int ret = nextc_validate(_nextc);
	num_out_t o = {0};
	o.use_base = 10;
	if ( ret != 0 ) return 1;
	void rdUint() {
		void cropH() {
			ret = 1;
			if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ) return;
			o.num = (utfncmp( _nextc->c, _nextc->p, 0 ) == 0) ?
				(uchar)(o.num) : (ushort)(o.num);
			o.bitcount = (utfncmp( _nextc->c, _nextc->p, 0 ) == 0) ? CHAR_BIT : bitsof(short);
			ret = 0;
		}
		void cropL() {
			ret = 1;
			if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ) return;
			o.num = (utfncmp( _nextc->c, _nextc->p, 0 ) == 0) ?
				(ullong)(o.num) : (ulong)(o.num);
			o.bitcount = (utfncmp( _nextc->c, _nextc->p, 0 ) == 0) ?
				bitsof(sllong) : bitsof(long);
			ret = 0;
		}
		void cropI() {
			uint_least64_t size = 0;
			uint_least64_t cropV() {
				uint_least64_t bit = -1;
				bit <<= size;
				o.num &= (~bit);
				ret = 0;
				return o.num;
			}
			switch (_nextc->c[0]) {
			case U'i': case U'I':
				ret = 1;
				if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ) return;
				if ( _nextc->c[0] >= U'0' && _nextc->c[0] <= U'9' )
					ret = rdU64_base62( _nextc, 10, 0, &size );
				else ret = 0;
			}
			switch ( size ) {
			case 64: break;
			case 32: o.num = (uint_least32_t)(o.num); break;
			case 16: o.num = (uint_least16_t)(o.num); break;
			case 8: o.num = (uint_least8_t)(o.num); break;
			case 0: o.num = (uint)(o.num); size = bitsof(int); break;
			default: o.num = cropV();
			}
			o.bitcount = size;
			ret = 0;
		}
		ret = 1;
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ) return;
		switch ( _nextc->c[0] ) {
		case U'x': case U'X': o.use_base = 16; break;
		case U'b': case U'B': o.use_base = 2; break;
		default: o.use_base = 8;
		}
		if (o.use_base != 8 && (ret = nextc(_nextc)) != EXIT_SUCCESS ) return;
		ret = rdU64_base62( _nextc, o.use_base, 0, &(o.num) );
		if ( ret != 0 ) return;
		if ( _nextc->c[0] == U'u' || _nextc->c[0] == U'U' ) {
			ret = 1;
			if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ) return;
			o.unSigned = 1;
		}
		switch (_nextc->c[0]) {
		case U'h': case U'H': return cropH();
		case U'l': case U'L': return cropL();
		}
		cropI();
	}
	void rdVal() {
		if (_nextc->c[0] == U'0') rdUint();
		else ret = rdU64_base62( _nextc, 10, 0, &(o.num) );
	}
	void rdInt() {
		ret = 1;
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ) return;
		o.isSigned = 1;
		rdVal();
		o.num = (_nextc->p[0] == U'-') ? -(o.num) : o.num;
	}
	if (_nextc->c[0] == U'-' || _nextc->c[0] == U'+') rdInt();
	else rdVal();
	if ( out ) *out = o;
	return ret;
}

int printNum( UTF _utf ) {
	int ret;
	num_out_t num_out = {0};
	NEXTC _nextc = {0};
	char *rdNumU = "rdNum(): %" PRIuLEAST64 "\n",
		*rdNumS = "rdNum(): %" PRIdLEAST64 "\n";
	_utf.pos = 0;
	_nextc.src = &_utf;
	_nextc.tellpos = (func_tellpos)utftellpos;
	_nextc.seekpos = (func_seekpos)utfseekpos;
	_nextc.nextchr = (func_nextchr)utfnextc;
	if ( (ret = nextc(&_nextc)) != EXIT_SUCCESS ) return ret;
	(void)rdNum( &_nextc, &num_out );
	(void)printf( num_out.unSigned ? rdNumU : rdNumS, num_out.num );
	return EXIT_SUCCESS;
}
int printNum2( UTF _utf ) {
	int ret;
	num_out_t num_out = {0};
	NEXTC _nextc = {0};
	char *rdNumU = "rdNum2(): %" PRIuLEAST64 "\n",
		*rdNumS = "rdNum2(): %" PRIdLEAST64 "\n";
	_utf.pos = 0;
	_nextc.src = &_utf;
	_nextc.tellpos = (func_tellpos)utftellpos;
	_nextc.seekpos = (func_seekpos)utfseekpos;
	_nextc.nextchr = (func_nextchr)utfnextc;
	if ( (ret = nextc(&_nextc)) != EXIT_SUCCESS ) return ret;
	(void)rdNum2( &_nextc, &num_out );
	(void)printf( num_out.unSigned ? rdNumU : rdNumS, num_out.num );
	return EXIT_SUCCESS;
}

tsc_t printWhich( UTF _utf,
	char8_t *text, size_t max, char *func,
	int (*rdnum)( NEXTC *_nextc, num_out_t *num_out ) )
{
	int ret;
	size_t i;
	tsc_t cinit, cstop;
	num_out_t num_out;
	char *rdNumU = "%s(): %" PRIuLEAST64 "\n";
	char *rdNumD = "%s(): %" PRIdLEAST64 "\n";
	NEXTC _nextc = {0};
	_nextc.src = &_utf;
	_nextc.tellpos = (func_tellpos)utftellpos;
	_nextc.seekpos = (func_seekpos)utfseekpos;
	_nextc.nextchr = (func_nextchr)utfnextc;
	printf("Testing %s()\n", func);
	cinit = tsc();
	for ( i = 0; i < max; ++i ) {
		(void)sprintf( text, "%d", rand() );
		_utf.len = utflen( text );
		(void)printf( "%s, ", text );
		_utf.pos = 0;
		(void)memset( &num_out, 0, sizeof(num_out_t) );
		if ( (ret = nextc(&_nextc)) != EXIT_SUCCESS ) return ret;
		rdnum( &_nextc, &num_out );
		(void)printf( num_out.unSigned ?
			rdNumU : rdNumD, func, num_out.num );
	}
	cstop = tsc();
	return cstop - cinit;
}

int main() {
	uint max = 3;
	tsc_t took1, took2;
	char8_t text[65] = {0};
	UTF _utf = {0};
	NEXTC _nextc = {0};
	_nextc.src = &_utf;
	_nextc.tellpos = (func_tellpos)utftellpos;
	_nextc.seekpos = (func_seekpos)utfseekpos;
	_nextc.nextchr = (func_nextchr)utfnextc;
	_utf.txt = (char8_t*)text;
	_utf.cap = 65;
	(void)sprintf( text, "%d", rand() );
	_utf.len = utflen( text );
	(void)printf( "Random Number Generated: '%s'\n", text );
	printNum( _utf );
	printNum2( _utf );
	puts("Testing sgetc()");
	_utf.pos = 0;
	while ( nextc(&_nextc) ) {
		if ( _nextc.c[0] ) fputc( _nextc.c[0], stdout );
		else (void)printf("#%ld", _utf.pos);
	}
	fputc( '\n', stdout );
	if ( _utf.pos < _utf.len ) return 1;
	took1 = printWhich( _utf, text, max, u8"rdNum", rdNum );
	took2 = printWhich( _utf, text, max, u8"rdNum2", rdNum2 );
	printf("Cycles: rdNum() %llu, rdNum2() %llu\n",
		(ullong)took1, (ullong)took2 );
	return 0;
}
