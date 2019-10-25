#include "tsc.h"
#include "next.h"
#include "base62.h"
#include <stdlib.h>
#include <stdio.h>
char const *std_encoding = NULL;
#define iconv_null ((iconv_t)-1)
iconv_t cd_null = iconv_null,
	std2utf = iconv_null, utf2std = iconv_null,
	c16_2_utf = iconv_null, utf_2_c16 = iconv_null,
	c32_2_utf = iconv_null, utf_2_c32 = iconv_null;
int get_std_encoding() {
	int ret = open_std_encodings();
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = std_encoding_u16;
	c16_2_utf = iconv_to_utf[ret];
	utf_2_c16 = utf_to_iconv[ret];
	ret = std_encoding_u32;
	c32_2_utf = iconv_to_utf[ret];
	utf_2_c32 = utf_to_iconv[ret];
	switch ( std_encoding_type ) {
	case '8': ret = std_encoding_utf; break;
	case 'u': ret = std_encoding_u16; break;
	case 'U': ret = std_encoding_u32; break;
	default: ret = std_encoding_std;
	}
	std_encoding = std_encoding_names[ret];
	std2utf = iconv_to_utf[ret];
	utf2std = utf_to_iconv[ret];
	return EXIT_SUCCESS;
}

int ishex( char32_t c ) {
	return ((c >= U'0' && c <='9')
				|| (c >= U'A' && c <= U'F')
				|| (c >= U'a' && c <= U'f'));
}

int rdEscChr( NEXTC *_nextc, UTFCHAR c ) {
	int ret = EXIT_SUCCESS;
	uint_least64_t num = 0;
	UTFCHAR now = {0};
	char *iconvSrcBuff = (char*)&num, *iconvDstBuff = (char*)now;
	size_t iconvSrcSize = sizeof(uint_least64_t),
		iconvDstSize = NEXTC_C_SIZE;
	_Bool readmore = 1;
	char const *esc, def[] = "\\";
	if ( !_nextc || !c ) {
		ret = EDESTADDRREQ;
		FAIL( stderr, ret, "_nextc and/or c was NULL" );
		if ( !c ) return ret;
		goto rdEscChr_done;
	}
	esc = getenv("ESCAPE_CHAR");
	if ( !esc ) esc = def;
	if ( _nextc->c[0] != esc[0] ) {
		ret = EILSEQ;
		FAIL( stderr, ret, "Invalid escape character" );
		(void)fprintf( stderr, "Expected '%c' got '%c' \n", esc[0], _nextc->c[0] );
		goto rdEscChr_done;
	}
	if ( (ret = nextc(_nextc)) != EXIT_SUCCESS )
		goto rdEscChr_done;
	switch ( _nextc->c[0] ) {
	case U'a': now[0] = 0x07u; break;
	case U'b': now[0] = 0x08u; break;
	case U'e': now[0] = 0x1Bu; break;
	case U'f': now[0] = 0x0Cu; break;
	case U'n': now[0] = 0x0Au; break;
	case U'r': now[0] = 0x0Du; break;
	case U't': now[0] = 0x09u; break;
	case U'u':
		readmore = 0;
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS )
			return ret;
		ret = ubase62( _nextc, &num, 16, 0, 4, 4 );
		(void)iconv( iconv_to_utf[std_encoding_u16],
			&iconvSrcBuff, &iconvSrcSize, &iconvDstBuff, &iconvDstSize );
		break;
	case U'U':
		readmore = 0;
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS )
			return ret;
		ret = ubase62( _nextc, &num, 16, 0, 8, 8 );
		(void)iconv( iconv_to_utf[std_encoding_u32],
			&iconvSrcBuff, &iconvSrcSize, &iconvDstBuff, &iconvDstSize );
		break;
	case U'v': now[0] = 0x0Bu; break;
	case U'x':
		readmore = 0;
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS )
			return ret;
		ret = ubase62( _nextc, &num, 16, 0, 2, 2 );
		now[0] = num;
		break;
	case U'0':
		readmore = 0;
		ret = ubase62( _nextc, &num, 8, 0, 1, 4 );
		now[0] = num;
		break;
	default:
		(void)utfcpy( now, NEXTC_C_LENG, _nextc->c, NEXTC_C_LENG );
	}
	rdEscChr_done:
	utfcpy( c, NEXTC_C_LENG, now, utfclen(now[0]) + 1 );
	if ( ret != EXIT_SUCCESS )
		return ret;
	if ( readmore )
		return nextc(_nextc);
	return EXIT_SUCCESS;
}
int istrcmp( char const * str, char const * val ) {
	int s, v;
	if ( !str ) return val ? -1 : 0;
	if ( !val ) return 1;
	while ( *str && *val ) {
		if ( *str == 0 ) return *val ? -1 : 0;
		if ( *val == 0 ) return 1;
		if ( *str != *val ) {
			s = tolower(*str);
			v = tolower(*val);
			if ( s != v ) return s - v;
		}
		++str;
		++val;
	}
	s = *str;
	v = *val;
	return s - v;
}
typedef struct rdTextTok {
	size_t i;
	size_t left;
	_Bool use_current;
	UTFCHAR now;
} rdTextTok;
int rdText( NEXTC *_nextc, char8_t *c, size_t leng, size_t stop, rdTextTok *tok ) {
	int ret = nextc_validate(_nextc);
	size_t max;
	if ( ret != EXIT_SUCCESS ) return ret;
	if ( !c || !tok ) {
		ret = EDESTADDRREQ;
		FAIL( stderr, ret, "c or tok was NULL!");
		return ret;
	}
	if ( !stop ) stop = leng;
	tok->left = leng - tok->i;
	if ( tok->now[0] ) {
		max = utfclen(tok->now[0]) + 1;
		if ( tok->left < max || stop <= (tok->i + max) )
			return ERANGE;
		(void)utfcpy( &c[tok->i], tok->left, tok->now, max );
		tok->i += max;
		tok->left -= max;
		(void)memset( tok->now, 0, NEXTC_C_SIZE );
	}
	while ( tok->use_current || (ret = nextc(_nextc)) == EXIT_SUCCESS ) {
		switch ( _nextc->c[0] ) {
		case 0: case U'\n': case U'\r':
			ret = EILSEQ;
			FAIL( stderr, ret, "special characters"
				" must be escaped in text literals!");
			return ret;
		case U'\\':
			(void)rdEscChr( _nextc, tok->now );
			tok->use_current = 1;
			max = utfclen(tok->now[0]) + 1;
			if ( tok->left < max || stop <= (tok->i + max) )
				return ERANGE;
			(void)utfcpy( &(c[tok->i]), tok->left, tok->now, max );
			(void)memset( tok->now, 0, NEXTC_C_SIZE );
			break;
		case U'\'': case U'"':
			if ( !(tok->left) ) {
				tok->use_current = 1;
				return ERANGE;
			}
			tok->use_current = 0;
			c[tok->i] = 0;
			return EXIT_SUCCESS;
		default:
			max = utfclen(_nextc->c[0]) + 1;
			if ( tok->left < max || stop <= (tok->i + max) ) {
				tok->use_current = 1;
				return ERANGE;
			}
			(void)utfcpy( &c[tok->i], tok->left, _nextc->c, max );
			tok->use_current = 0;
		}
		tok->i += max;
		tok->left = leng - tok->i;
	}
	return ret;
}
int FlipBytes( char *block, size_t size ) {
	char tmp, *end;
	if ( !block ) return EDESTADDRREQ;
	if ( !size ) return ERANGE;
	for ( end = &(block[size-1]); end > block; ++block, --end ) {
		tmp = *block;
		*block = *end;
		*end = tmp;
	}
	return EXIT_SUCCESS;
}
int FlipHalves( short *block, size_t size ) {
	short tmp, *end;
	if ( !block ) return EDESTADDRREQ;
	if ( !size ) return ERANGE;
	for ( end = &(block[(size/sizeof(short))-1]); end > block; ++block, --end ) {
		tmp = *block;
		*block = *end;
		*end = tmp;
	}
	return EXIT_SUCCESS;
}
#define NATIVEC_LENG 31
#define NATIVEC_SIZE (NATIVEC_LENG+1)
bool isutfwhitespace( char32_t c ) {
	switch ( c ) {
	case U'\r': case U'\n':
	case U'\f': case U'\v':
	case U'\t': case U' ':
		return 1;
	}
	return 0;
}
int readwhitespace( NEXTC *_nextc ) {
	int ret;
	while ( (ret = nextc(_nextc)) == EXIT_SUCCESS ) {
		switch ( _nextc->c[0] ) {
		case U'\r': case U'\n':
		case U'\f': case U'\v':
		case U'\t': case U' ':
			continue;
		}
		return EXIT_SUCCESS;
	}
	return ret;
}
typedef struct vec {
	size_t size, upto, used, node;
	void *buff;
} VEC;
int vecsize( VEC *_vec, size_t new_size ) {
	int ret;
	VEC vec;
	if ( !_vec ) {
		ret = EDESTADDRREQ;
		FAIL( stderr, ret, "" );
		return ret;
	}
	if ( !new_size ) {
		if ( _vec->buff )
			free(_vec->buff);
		(void)memset( _vec, 0, sizeof(VEC) );
		return EXIT_SUCCESS;
	}
	vec = *_vec;
	vec.buff = realloc( vec.buff, new_size );
	if ( !vec.buff ) {
		ret = errno;
		errno = EXIT_SUCCESS;
		return ret;
	}
	if ( new_size > vec.size )
		memset( &(((char*)vec.buff)[vec.size]), 0, new_size - vec.size );
	vec.size = new_size;
	vec.upto = new_size / vec.node;
	if ( vec.upto < vec.used )
		vec.used = vec.upto;
	*_vec = vec;
	return EXIT_SUCCESS;
}
#define UTF_INC_CHAR (BUFSIZ / sizeof(char8_t))
int literal_type( NEXTC *_nextc, char32_t *type ) {
	int ret = EXIT_SUCCESS;
	if ( !type ) {
		ret = EDESTADDRREQ;
		FAIL( stderr, ret, "" );
		return ret;
	}
	switch ( _nextc->c[0] ) {
	case U'"': case U'\'': case U'\\':
		*type = _nextc->c[0];
		break;
	case 0:
		ret = ENODATA;
	default:
		*type = 0;
	}
	return ret;
}
int literal_pfx( NEXTC *_nextc, char32_t *type, char32_t *pfx ) {
	int ret, c;
	char32_t t, p = 0;
	if ( !type || !pfx ) {
		ret = EDESTADDRREQ;
		FAIL( stderr, ret, "" );
		return ret;
	}
	ret = literal_type(_nextc, type);
	if ( *type || ret != EXIT_SUCCESS ) {
		*pfx = 0;
		return ret;
	}
	switch ( c = _nextc->c[0] ) {
	case 0: ret = ENODATA; break;
	case U'U': case U'L': case U'b': case U'B':
		p = c;
		ret = nextc(_nextc);
		break;
	case U'u':
		ret = nextc(_nextc);
		if ( _nextc->c[0] == '8' ) {
			p = U'8';
			ret = nextc(_nextc);
			break;
		}
		p = c;
		break;
	default:
		putchar('\n');
		ret = EILSEQ;
		FAIL( stderr, ret, "" );
		fprintf( stderr, "c = '%c'\n", c );
	}
	switch ( p ) {
	case U'8': case U'u': case U'U': case U'L':
		ret = literal_type( _nextc, &t );
		if ( t != U'"' && t != U'\'' )
			ret = EILSEQ;
	}
	*type = t;
	*pfx = p;
	return ret;
}
typedef struct _literaln {
	char core;
	char type[4];
	size_t base;
	uint_fast64_t width;
	uint_least64_t num;
	long double fpn;
} LITERALN;
/*** @brief Attempts to read from current character a name,
 * if the name can be found in src then that name should turned
 * into a number or ->type[1] set to '#' to indicate it is a variable
 * to be loaded at runtime, if function paramters use a different load
 * method use '@' instead to indicate as such
 * @return EXIT_SUCCESS or error code
 * **/
typedef int (*func_findnumber)( NEXTC *_nextc, LITERALN *dst, void *src );
int literaln_prefix( NEXTC *_nextc, LITERALN *_literaln ) {
	int ret = nextc_validate(_nextc), c;
	if ( ret != EXIT_SUCCESS ) return ret;
	if ( !_literaln ) {
		ret = EDESTADDRREQ;
		FAIL( stderr, ret, "" );
		return ret;
	}
	(void)memset( _literaln, 0, sizeof(LITERALN) );
	while ( ret == EXIT_SUCCESS ) {
		switch ( c = _nextc->c[0] ) {
		case U'-':
			_literaln->type[0] =
				( _literaln->type[0] == U'-' ) ? U'+' : U'-';
			ret = readwhitespace(_nextc);
			break;
		case U'+':
			_literaln->type[0] =
				( _literaln->type[0] == U'-' ) ? U'-' : U'+';
			ret = readwhitespace(_nextc);
			break;
		default:
			return ret;
		}
	}
	return ret;
}
typedef union _hfn {
	uint64_t sig : 1;
	uint64_t exp : 7;
	uint64_t man : 8;
} UHFN;
typedef union _flt {
	uint64_t sig : 1;
	uint64_t exp : 7;
	uint64_t man : 24;
} UFLT;
typedef union _udbl {
	uint64_t sig : 1;
	uint64_t exp : 15;
	uint64_t man : 48;
} UDBL;
typedef union _uldb {
	uint16_t sig : 1;
	uint16_t exp : 15;
	uint64_t man : 64;
} ULDB;
int literaln_fpn( NEXTC *_nextc, LITERALN *_literaln ) {
	int ret = nextc_validate(_nextc), c;
	uint_least64_t exp;
	size_t bits_per_digit;
	size_t exp_size = (sizeof(uint_least64_t) / 4);
	size_t exp_width = (exp_size * CHAR_BIT);
	size_t man_width = bitsof(uint_least64_t) - exp_width;
	c = _nextc->c[0];
	switch ( c ) {
	case U'p': case U'p':
		_literaln->type[1] = 'p';
		if ( _literaln->core != U'x' )
			ret = EILSEQ;
			FAIL("p & P are only used in hexadecimal floating numbers");
			return ret;
		}
		break;
	case U'e': case U'E':
		if ( _literaln->core && _literaln->core != 'b' ) {
			ret = EILSEQ;
			FAIL("e & E are only used in decimal & binary floating numbers");
			return ret;
		}
		break;
	case U'`':
		_literaln->type[1] = '`';
		if ( _literaln.core != '#' ) {
			ret = EILSEQ;
			FAIL("` is only used in base floating numbers");
			return ret;
		}
	}
	bits_per_digit = _literaln.base / 4;
	ret = nextc(_nextc);
	ret = ubase62( _nextc, &exp, _literaln->base, _literaln->lowislow, 1,
		(bitsof(uint_least64_t) / bits_per_digit) +
		(bitsof(uint_least64_t) % bits_per_digit) );
}
int literaln_sfx( NEXTC *_nextc, LITERALN *_literaln ) {
	int ret = nextc_validate(_nextc), c;
	uint_least64_t bit, bits;
	if ( ret != EXIT_SUCCESS ) return ret;
	if ( !_literaln ) {
		ret = EDESTADDRREQ;
		FAIL( stderr, ret, "" );
		return ret;
	}
	switch ( _nextc->c[0] ) {
	case U'u': case U'U':
		_literaln->type[1] = U'u';
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS && ret != ENODATA )
			return ret;
	}
	switch ( c = _nextc->c[0] ) {
	case U'h': case U'H':
		_literaln->type[2] = U'h';
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS && ret != ENODATA )
			return ret;
		if ( _nextc->c[0] == U'h' || _nextc->c[0] == U'H' ) {
			_literaln->type[3] = U'h';
			_literaln->width = CHAR_BIT;
			ret = nextc(_nextc);
		}
		else
			_literaln->width = bitsof(short);
		break;
	case U'l': case U'L':
		_literaln->type[2] = U'l';
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS && ret != ENODATA )
			return ret;
		if ( _nextc->c[0] == U'l' || _nextc->c[0] == U'L' ) {
			_literaln->type[3] = U'l';
			_literaln->width = bitsof(sllong);
			ret = nextc(_nextc);
		}
		else
			_literaln->width = bitsof(long);
		break;
	case U'i': case U'I':
		_literaln->type[2] = U'i';
		if ( (ret = nextc(_nextc)) == EXIT_SUCCESS )
			ret = ubase62( _nextc, &(_literaln->width), 10, 0, 1,
				(bitsof(uint_least64_t) / 3) + (bitsof(uint_least64_t) % 3) );
		else
			_literaln->width = bitsof(int);
		break;
	default:
		_literaln->width = bitsof(int);
	}
	for (	bit = 0, bits = 0;
		bits < _literaln->width;
		++bits, bit <<= 1, bit |= 1u );
	if ( _literaln->type[0] == U'-' )
		_literaln->num = -(_literaln->num);
	_literaln->num ^= (_literaln->num & ~bit);
	return ret;
}
int literaln( NEXTC *_nextc, LITERALN *_literaln, func_findnumber find, void *src ) {
	int ret = literaln_prefix(_nextc,_literaln), c;
	size_t max;
	if ( ret != EXIT_SUCCESS ) return ret;
	c = _nextc->c[0];
	if ( c < U'0' || c > U'9' ) {
		if ( _literaln->type[0] ) {
			if ( find &&
				(ret = find( _nextc, _literaln, src )) == EXIT_SUCCESS )
				return ret;
			FAIL( stderr, ret, "+/- can only be used on numbers");
			return ret;
		}
		return EXIT_FAILURE;
	}
	if ( c == U'0' ) {
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS )
			return ret;
		switch ( c = _nextc->c[0] ) {
		case U'b': case U'B':
			_literaln->core = U'b';
			_literaln->base = 2;
			max = bitsof(uint_least64_t);
			if ( (ret = nextc(_nextc)) != EXIT_SUCCESS )
				return ret;
			break;
		case U'x': case U'X':
			_literaln->core = U'x';
			_literaln->base = 16;
			max = bitsof(uint_least64_t) / 4;
			if ( (ret = nextc(_nextc)) != EXIT_SUCCESS )
				return ret;
			break;
		default:
			if ( c >= U'0' && c <= U'9' ) {
				_literaln->core = U'0';
				_literaln->base = 8;
				max = bitsof(uint_least64_t) / 3;
				break;
			}
			return EXIT_SUCCESS;
		}
	}
	else {
		_literaln->base = 10;
		max = (bitsof(uint_least64_t) / 3) + (bitsof(uint_least64_t) % 3);
	}
	if ( (ret = ubase62( _nextc, &(_literaln->num),
		_literaln->base, 0, 1, max )) != EXIT_SUCCESS && ret != ENODATA )
		return ret;
	return literaln_suffix(_nextc,_literaln);
}
typedef int (*func_putc)( void *src, int c );
void puti( FILE *file, LITERALN _literaln ) {
	uint_least64_t bit;
	size_t i = 3;
	char format[30] = "%lluui%llu (unsigned)";
	if ( _literaln.type[1] == U'u' )
		i += 2;
	else {
		format[i++] = 'd';
		format[11] = ' ';
		format[12] = ' ';
		format[13] = '(';
		bit = 1u << (_literaln.width - 1);
		if ( _literaln.num & bit ) {
			while ( (bit <<= 1) )
				_literaln.num |= bit;
		}
	}
	if ( _literaln.type[2] == U'i' ) {
		fprintf(file, format,
			(ullong)(_literaln.num), (ullong)(_literaln.width) );
		return;
	}
	switch ( _literaln.type[2] ) {
	case U'h': format[i++] = 'h'; break;
	case U'l': format[i++] = 'l'; break;
	}
	switch ( _literaln.type[3] ) {
	case U'h': format[i++] = 'h'; break;
	case U'l': format[i++] = 'l'; break;
	}
	while ( i < 10 ) format[i++] = ' ';
	fprintf(file, format, (ullong)(_literaln.num) );
}
int literalc( NEXTC *_nextc, bool allownewlines ) {
	int ret, encoding, c;
	char8_t *u8 = NULL;
	char *cb = getenv("CHAR_IS_LE"), *iconvDstBuff, *iconvSrcBuff;
	size_t iconvDstSize, iconvSrcSize, i, bytes;
	uint_least64_t base;
	_Bool lowislow = 0, mbIsLE =
		(!cb || strcmp(cb,"0") == 0 || istrcmp(cb,"false") == 0) ? 0 : 1;
	VEC U8 = {0}, CB = {0};
	rdTextTok tok = {0};
	char32_t _type, pfx;
	LITERALN _literaln = {0};
	if ( (ret = literaln( _nextc, &_literaln, NULL, NULL ))
		!= EXIT_FAILURE ) {
		switch ( ret ) {
		case EXIT_SUCCESS: case ENODATA: break;
		default: return ret;
		}
		puti( stdout, _literaln );
		return ret;
	}
	ret = literal_pfx( _nextc, &_type, &pfx );
	if ( ret != EXIT_SUCCESS ) return ret;
	switch ( pfx ) {
	case U'b': case U'B':
		if ( (ret = ubase62( _nextc, &base, 10, 0, 1, 2 )) != EXIT_SUCCESS )
			return ret;
		if ( base < 2 || base > 62 ) {
			ret = ERANGE;
			FAIL( stderr, ret, "Base must be between 2 & 62" );
			return ret;
		}
		if ( _nextc->c[0] == U'l' || _nextc->c[0] == U'L' ) {
			lowislow = 1;
			if ( (ret = nextc(_nextc)) )
				return ret;
		}
		if ( _nextc->c[0] != U'"' ) {
			ret = EILSEQ;
			FAIL( stderr, ret, "Base literals must be contained in a \" characters" );
			return ret;
		}
		if ( (ret = nextc(_nextc)) != EXIT_SUCCESS ||
			(ret = literaln_prefix(_nextc,&_literaln)) != EXIT_SUCCESS ||
			(ret = ubase62(_nextc,&_literaln.num,base,lowislow,1,0)) != EXIT_SUCCESS )
			return ret;
		if ( _nextc->c[0] != U'"' ) {
			ret = EILSEQ;
			FAIL( stderr, ret, "Base literals must be contained in a \" characters" );
			return ret;
		}
		puts("puts7");
		(void)nextc(_nextc);
		ret = literaln_suffix(_nextc,&_literaln);
		puti(stdout,_literaln);
		return ret;
	case U'8': encoding = std_encoding_utf; break;
	case U'u': encoding = std_encoding_u16; break;
	case U'U': encoding = std_encoding_u32; break;
	case U'L': encoding = std_encoding_wcs; break;
	default: encoding = std_encoding_node;
	}
	U8.node = sizeof(char8_t);
	CB.node = 1;
	ret = vecsize( &U8, BUFSIZ );
	if ( ret != EXIT_SUCCESS ) return ret;
	u8 = U8.buff;
	cb = NULL;
	putchar(' ');
	if ( _type == U'"' ) {
		while ( (ret = rdText(
			_nextc, u8, U8.upto - 1, 0, &tok )) == ERANGE )
		{
			ret = vecsize( &U8, U8.size + BUFSIZ );
			if ( ret != EXIT_SUCCESS ) goto literalc_cleanup;
			u8 = U8.buff;
		}
		if ( _nextc->c[0] != U'"' ) {
			ret = EILSEQ;
			FAIL( stderr, ret, "String literals must close with \"" );
			goto literalc_cleanup;
		}
		if ( ret != EXIT_SUCCESS )
			goto literalc_cleanup;
	}
	else if ( _type == U'\'' ) {
		ret = rdText( _nextc, u8, U8.upto - 1, 5, &tok );
		if ( ret != EXIT_SUCCESS )
			goto literalc_cleanup;
		if ( _nextc->c[0] != '\'' ) {
			ret = EILSEQ;
			FAIL( stderr, ret, "Characters must close with '" );
		}
	}
	else if ( _type == U'\\' )
		ret = rdEscChr( _nextc, u8 );
	else
		(void)utfcpy( u8, NEXTC_C_LENG, _nextc->c, NEXTC_C_LENG );
	if ( ret != EXIT_SUCCESS )
		goto literalc_cleanup;
	U8.used = utflen(u8) + 1;
	if ( (ret = vecsize( &CB, U8.size * 4 )) != EXIT_SUCCESS )
		goto literalc_cleanup;
	cb = CB.buff;
	iconvSrcBuff = U8.buff;
	iconvSrcSize = U8.used * sizeof(char8_t);
	iconvDstBuff = CB.buff;
	iconvDstSize = CB.size;
	if ( encoding != std_encoding_utf ) {
		errno = EXIT_SUCCESS;
		while ( iconvSrcSize && iconvDstSize ) {
			bytes = iconv( utf_to_iconv[encoding],
				&iconvSrcBuff, &iconvSrcSize,
				&iconvDstBuff, &iconvDstSize );
			if ( !bytes ) break;
			if ( errno != EXIT_SUCCESS ) break;
		}
		if ( errno != EXIT_SUCCESS ) {
			ret = errno;
			errno = EXIT_SUCCESS;
			FAIL( stderr, ret, "" );
		}
	}
	else (void)utfcpy( cb, U8.used, u8, U8.used );
	if ( _type == U'\'' && !mbIsLE ) {
		CB.used = !!cb[0] + !!cb[1] + !!cb[2] + !!cb[3];
		if ( (ret = FlipBytes( cb, CB.used )) != EXIT_SUCCESS )
			goto literalc_cleanup;
	}
	printf( "%08X, '", *((char32_t*)cb) );
	switch ( encoding ) {
	case std_encoding_wcs:
		for ( i = 0; (c = ((wchar_t*)cb)[i]); ++i )
			putwchar( c );
		putchar('\'');
		break;
	case std_encoding_u16:
		for ( i = 0; (c = ((char16_t*)cb)[i]); ++i )
			putwchar( c );
		putchar('\'');
		break;
	case std_encoding_u32:
		for ( i = 0; (c = ((char32_t*)cb)[i]); ++i )
			putwchar( c );
		putchar('\'');
		break;
	default:
		printf( "%s'", cb );
	}
	literalc_cleanup:
	vecsize( &U8, 0 );
	vecsize( &CB, 0 );
	return ret;
}
int literalc_utf( char8_t *txt ) {
	int ret;
	UTF _utf = {0};
	NEXTC _nextc = {0};
	char cb[NATIVEC_SIZE] = {0},
		*iconvDstBuff = cb, *iconvSrcBuff = (char*)txt;
	size_t iconvSrcSize, iconvDstSize = NATIVEC_SIZE;
	_nextc.tellpos = (func_tellpos)utftellpos;
	_nextc.seekpos = (func_seekpos)utfseekpos;
	_nextc.nextchr = (func_nextchr)utfnextc;
	_nextc.src = &_utf;
	_utf.txt = txt;
	_utf.len = utflen( txt );
	_utf.cap = _utf.len + 1;
	if ( std_encoding_node == std_encoding_utf )
		(void)printf("%s = ", txt );
	else {
		iconvSrcSize = _utf.cap * sizeof(char8_t);
		iconv( utf_to_iconv[std_encoding_std],
			&iconvSrcBuff, &iconvSrcSize, &iconvDstBuff, &iconvDstSize );
		printf("%s = ", cb );
	}
	nextc(&_nextc);
	ret = literalc( &_nextc, 1 );
	putchar('\n');
	return (ret == ENODATA) ? EXIT_SUCCESS : ret;
}
char8_t *unit_test_characters[] = {
	u8"010",
	u8"0b101",
	u8"99999",
	u8"0x5f5e0ff",
	u8"-010",
	u8"-0b101",
	u8"-99999",
	u8"-0x5f5e0ff",
	u8"-1u",
	u8"-1uhh",
	u8"-1ull",
	u8"-1ui10",
	u8"-99",
	u8"-99hh",
	u8"-99ll",
	u8"-99i10",
	u8"0xFFFFFFFFFFFFFFFF",
	u8"0xFFFFFFFFFFFFFFFFh",
	u8"0xFFFFFFFFFFFFFFFFhh",
	u8"0xFFFFFFFFFFFFFFFFl",
	u8"0xFFFFFFFFFFFFFFFFll",
	u8"0xFFFFFFFFFFFFFFFFi9",
	u8"0xFFFFFFFFFFFFFFFFu",
	u8"0xFFFFFFFFFFFFFFFFuh",
	u8"0xFFFFFFFFFFFFFFFFuhh",
	u8"0xFFFFFFFFFFFFFFFFul",
	u8"0xFFFFFFFFFFFFFFFFull",
	u8"0xFFFFFFFFFFFFFFFFui9",
	u8"b35\"z\"",
	u8"b62\"z\"",
	u8"b16\"fff\"hh",
	u8"b16\"fff\"hH",
	u8"\\u2ea2",
	u8"'abcd'",
	u8"'\\u2ea2'",
	u8"'a\\u2ea2'",
	u8"'\\u2ea2a'",
	u8"\"\\u2ea2\"",
	u8"\"abcd\\u2ea2\"",
	u8"\"\\u2ea2abcd\"",
	u8"\"abcd\\u2ea2wxyz\"",
	u8"u8'abcd'",
	u8"u8'\\u2ea2'",
	u8"u8'a\\u2ea2'",
	u8"u8'\\u2ea2a'",
	u8"u8\"\\u2ea2\"",
	u8"u8\"abcd\\u2ea2\"",
	u8"u8\"\\u2ea2abcd\"",
	u8"u8\"abcd\\u2ea2wxyz\"",
	u8"u'cd'",
	u8"u'\\u2ea2'",
	u8"u'a\\u2ea2'",
	u8"u'\\u2ea2a'",
	u8"u\"\\u2ea2\"",
	u8"u\"abcd\\u2ea2\"",
	u8"u\"\\u2ea2abcd\"",
	u8"u\"abcd\\u2ea2wxyz\"",
	u8"U'd'",
	u8"U'\\u2ea2'",
	u8"U\"\\u2ea2\"",
	u8"U\"abcd\\u2ea2\"",
	u8"U\"\\u2ea2abcd\"",
	u8"U\"abcd\\u2ea2wxyz\"",
	u8"L'abcd'",
	u8"L'\\u2ea2'",
	u8"L'a\\u2ea2'",
	u8"L'\\u2ea2a'",
	u8"L\"\\u2ea2\"",
	u8"L\"abcd\\u2ea2\"",
	u8"L\"\\u2ea2abcd\"",
	u8"L\"abcd\\u2ea2wxyz\"",
NULL };
void multibyte( char *txt, int c ) {
	char cb[NATIVEC_SIZE] = {0};
	(void)memcpy( cb, &c, sizeof(int) );
	(void)printf("'%s': 0x%08X, '%c', %s\n", txt, c, c, cb );
}
int main() {
	int i = U'\u2ea2', ret = 0;
	NEXTC _nextc = {0};
	if ( (ret = get_std_encoding()) != 0 ) {
		FAIL( stderr, ret, "Could'nt identify environment encoding" );
		return EXIT_FAILURE;
	}
	(void)printf("std_encoding = '%s'\n",std_encoding);
	(void)puts("Compiler Tests:");
	multibyte( "abcd", 'abcd' );
	multibyte( "\u2ea2", '\u2ea2' );
	multibyte( "a\u2ea2", 'a\u2ea2' );
	multibyte( "\u2ea2a", '\u2ea2a' );
	// literalc tests
	(void)puts("Unit tests:");
	for ( i = 0; unit_test_characters[i]; ++i ) {
		if ( (ret = literalc_utf( unit_test_characters[i] ))
			!= EXIT_SUCCESS ) goto main_close_std_iconv;
	}
	// Manual tests
	(void)puts("User Tests:");
	ret = EXIT_SUCCESS;
	(void)memset(&_nextc, 0, sizeof(NEXTC) );
	_nextc.tellpos = (func_tellpos)ftell;
	_nextc.seekpos = (func_seekpos)fseek;
	_nextc.nextchr = (func_nextchr)futfnextc;
	_nextc.src = stdin;
	for ( i = 0; i < 10; ++i ) {
		(void)fputs("Please enter a character:\n", stdout);
		ret = literalc( &_nextc, 0 );
		if ( ret != EXIT_SUCCESS ) break;
	}
	main_close_std_iconv:
	shut_std_encodings();
	switch ( ret ) {
	case EXIT_SUCCESS: case ENODATA: break;
	default:
		putchar('\n');
		(void)printf("Last character was '%*s'\n", NEXTC_C_UPTO, _nextc.c );
		(void)printf("Character hex:"
			" %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX \n",
			_nextc.c[0], _nextc.c[1], _nextc.c[2],
			_nextc.c[3], _nextc.c[4], _nextc.c[5] );
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
