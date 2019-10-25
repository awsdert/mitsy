#include "mcc_get.h"
int mcc_encoding_type = 0;
int mcc_encoding_node = mcc_encoding_count;
char const *env_LANG = NULL;
mcc_char_info_t mcc_common_char_info[ mcc_encoding_count ] = {{0}};
int mcc_char_info_test( mcc_char_info_t mcc_char_info ) {
	if ( !(mcc_char_info.size) ||
		mcc_char_info.size > sizeof(intmax_t) ) return ERANGE;
	if ( mcc_char_info.enc != mcc_encoding_ch8 ) {
		if ( mcc_char_info.enc >= mcc_encoding_count ) return ERANGE;
		if ( mcc_char_info.ch8dst < 0 || mcc_char_info.ch8src < 0 )
			return EINVAL;
	}
	if ( !mcc_char_info.name ||
		!mcc_char_info.cleng || !mcc_char_info.ctype )
		return EADDRNOTAVAIL;
	return EXIT_SUCCESS;
}
int find_mcc_encoding() {
	int ret = EADDRNOTAVAIL;
	mcc_encoding_t i;
	char const *env, *enc;
	if ( mcc_encoding_node < mcc_encoding_count )
		return mcc_common_char_info[mcc_encoding_str].name ? 0 : ret;
	for ( i = 0; i < mcc_encoding_count; ++i )
		mcc_common_char_info[i].enc = i;
	/* Sizes */
	mcc_common_char_info[mcc_encoding_str].size = 1;
	mcc_common_char_info[mcc_encoding_wcs].size = sizeof(wchar_t);
	mcc_common_char_info[mcc_encoding_ch8].size = sizeof(mcc_ch8_t);
	mcc_common_char_info[mcc_encoding_c16].size = sizeof(mcc_c16_t);
	mcc_common_char_info[mcc_encoding_c32].size = sizeof(mcc_c32_t);
	/* Character lengths */
	mcc_common_char_info[mcc_encoding_str].cleng = (func_mcc_cleng)mcc_strcleng;
	mcc_common_char_info[mcc_encoding_wcs].cleng = (func_mcc_cleng)mcc_wcscleng;
	mcc_common_char_info[mcc_encoding_ch8].cleng = (func_mcc_cleng)mcc_ch8cleng;
	mcc_common_char_info[mcc_encoding_c16].cleng = (func_mcc_cleng)mcc_c16cleng;
	mcc_common_char_info[mcc_encoding_c32].cleng = (func_mcc_cleng)mcc_c32cleng;
	/* Character types */
	mcc_common_char_info[mcc_encoding_str].ctype = (func_mcc_ctype)mcc_strctype;
	mcc_common_char_info[mcc_encoding_wcs].ctype = (func_mcc_ctype)mcc_wcsctype;
	mcc_common_char_info[mcc_encoding_ch8].ctype = (func_mcc_ctype)mcc_ch8ctype;
	mcc_common_char_info[mcc_encoding_c16].ctype = (func_mcc_ctype)mcc_c16ctype;
	mcc_common_char_info[mcc_encoding_c32].ctype = (func_mcc_ctype)mcc_c32ctype;
	/* Names */
	env = getenv("LANG");
	if ( !env ) {
		FAIL( stderr, ret, "getenv('LANG') returned NULL! Using en.CH8-8");
		env = "en.CH8-8";
	}
	env_LANG = env;
	enc = strstr(env,".");
	if ( !enc || enc == env ) {
		FAIL( stderr, ret, "strstr(env,'.') returned NULL or env");
		return ret;
	}
	mcc_common_char_info[mcc_encoding_str].name = ++enc;
	mcc_common_char_info[mcc_encoding_wcs].name = "WCHAR_T";
	if ( strcmp( enc, "UTF-" ) > 0 ) {
		mcc_common_char_info[mcc_encoding_ch8].name = "UTF-8";
		mcc_common_char_info[mcc_encoding_c16].name = "UTF-16";
		mcc_common_char_info[mcc_encoding_c32].name = "UTF-32";
	}
	else if ( strcmp( enc, "utf-" ) > 0 ) {
		mcc_common_char_info[mcc_encoding_ch8].name = "utf-8";
		mcc_common_char_info[mcc_encoding_c16].name = "utf-16";
		mcc_common_char_info[mcc_encoding_c32].name = "utf-32";
	}
	else if ( strcmp( enc, "UTF" ) > 0 ) {
		mcc_common_char_info[mcc_encoding_ch8].name = "UTF8";
		mcc_common_char_info[mcc_encoding_c16].name = "UTF16";
		mcc_common_char_info[mcc_encoding_c32].name = "UTF32";
	}
	else if ( strcmp( enc, "utf" ) > 0 ) {
		mcc_common_char_info[mcc_encoding_ch8].name = "utf8";
		mcc_common_char_info[mcc_encoding_c16].name = "utf16";
		mcc_common_char_info[mcc_encoding_c32].name = "utf32";
	}
	else {
		mcc_common_char_info[mcc_encoding_ch8].name = "UTF-8";
		mcc_common_char_info[mcc_encoding_c16].name = "UTF-16";
		mcc_common_char_info[mcc_encoding_c32].name = "UTF-32";
	}
	if ( strcmp(enc,mcc_common_char_info[mcc_encoding_wcs].name) == 0 ) {
		mcc_encoding_type = 'L';
		mcc_encoding_node = mcc_encoding_wcs;
	}
	else if ( strcmp(enc,mcc_common_char_info[mcc_encoding_ch8].name) == 0 ) {
		mcc_encoding_type = '8';
		mcc_encoding_node = mcc_encoding_ch8;
	}
	else if ( strcmp(enc,mcc_common_char_info[mcc_encoding_c16].name) == 0 ) {
		mcc_encoding_type = 'u';
		mcc_encoding_node = mcc_encoding_c16;
	}
	else if ( strcmp(enc,mcc_common_char_info[mcc_encoding_c32].name) == 0 ) {
		mcc_encoding_type = 'U';
		mcc_encoding_node = mcc_encoding_c32;
	}
	return EXIT_SUCCESS;
}
void shut_mcc_encodings() {
	size_t i;
	iconv_t tmp;
	for ( i = 0; i < mcc_encoding_count; ++i ) {
		tmp = mcc_common_char_info[i].ch8dst;
		mcc_common_char_info[i].ch8dst = invalid_iconv;
		if ( tmp != invalid_iconv && tmp != skip_iconv )
			iconv_close(tmp);
		tmp = mcc_common_char_info[i].ch8src;
		mcc_common_char_info[i].ch8src = invalid_iconv;
		if ( tmp != invalid_iconv && tmp != skip_iconv )
			iconv_close(tmp);
	}
}
int open_mcc_encodings() {
	int ret = find_mcc_encoding();
	size_t i;
	char const *dst, *src;
	if ( ret != 0 ) return ret;
	dst = mcc_common_char_info[mcc_encoding_ch8].name;
	for ( i = 0; i < mcc_encoding_count; ++i ) {
		src = mcc_common_char_info[i].name;
		mcc_common_char_info[i].ch8dst = ( strcmp( dst, src ) == 0 )
			? skip_iconv : iconv_open( dst, src );
		ret = errno;
		if ( ret != (errno = EXIT_SUCCESS) ) {
			FAIL( stderr, ret, "" );
			shut_mcc_encodings();
			return ret;
		}
	}
	src = mcc_common_char_info[mcc_encoding_ch8].name;
	for ( i = 0; i < mcc_encoding_count; ++i ) {
		dst = mcc_common_char_info[i].name;
		mcc_common_char_info[i].ch8src = ( strcmp( dst, src ) == 0 )
			? skip_iconv : iconv_open( dst, src );
		ret = errno;
		if ( ret != (errno = EXIT_SUCCESS) ) {
			FAIL( stderr, ret, "" );
			shut_mcc_encodings();
			return ret;
		}
	}
	return EXIT_SUCCESS;
}
int mcc_strctype( char c ) {
	switch ( c ) {
	case '\n': case '\r':
	case '\v': case '\f':
	case '\t': case ' ':
		return MCC_CHAR_TYPE_SPACE;
	case '!': case '"':
	case '#': case '$':
	case '%': case '&': case '\'':
	case '(': case ')': case '*':
	case '+': case ',': case '-':
	case '.': case '/':
	case ':': case ';':
	case '<': case '=': case '>':
	case '?': case '@': case '[':
	case '\\': case ']': case '^':
	case '_': case '`': case '{':
	case '|': case '}': case '~':
		return MCC_CHAR_TYPE_PUNCT;
	default:
		if ( c >= '0' && c <= '9' )
			return MCC_CHAR_TYPE_DIGIT;
		if ( c >= 'A' && c <= 'Z' )
			return MCC_CHAR_TYPE_UPPER;
		if ( c >= 'a' && c <= 'z' )
			return MCC_CHAR_TYPE_LOWER;
	}
	if ( c < 0x80 )	return MCC_CHAR_TYPE_ASCII;
	return MCC_CHAR_TYPE_OTHER;
}
int mcc_wcsctype( wchar_t c ) {
	switch ( c ) {
	case L'\n': case L'\r':
	case L'\v': case L'\f':
	case L'\t': case L' ':
		return MCC_CHAR_TYPE_SPACE;
	case L'!': case L'"':
	case L'#': case L'$':
	case L'%': case L'&': case L'\'':
	case L'(': case L')': case L'*':
	case L'+': case L',': case L'-':
	case L'.': case L'/':
	case L':': case L';':
	case L'<': case L'=': case L'>':
	case L'?': case L'@': case L'[':
	case L'\\': case L']': case L'^':
	case L'_': case L'`': case L'{':
	case L'|': case L'}': case L'~':
		return MCC_CHAR_TYPE_PUNCT;
	default:
		if ( c >= L'0' && c <= L'9' )
			return MCC_CHAR_TYPE_DIGIT;
		if ( c >= L'A' && c <= L'Z' )
			return MCC_CHAR_TYPE_UPPER;
		if ( c >= L'a' && c <= L'z' )
			return MCC_CHAR_TYPE_LOWER;
	}
	if ( c < 0x80 )	return MCC_CHAR_TYPE_ASCII;
	return MCC_CHAR_TYPE_OTHER;
}
int mcc_ch8ctype( mcc_ch8_t c ) {
	switch ( c ) {
	case U'\n': case U'\r':
	case U'\v': case U'\f':
	case U'\t': case U' ':
		return MCC_CHAR_TYPE_SPACE;
	case U'!': case U'"':
	case U'#': case U'$':
	case U'%': case U'&': case U'\'':
	case U'(': case U')': case U'*':
	case U'+': case U',': case U'-':
	case U'.': case U'/':
	case U':': case U';':
	case U'<': case U'=': case U'>':
	case U'?': case U'@': case U'[':
	case U'\\': case U']': case U'^':
	case U'_': case U'`': case U'{':
	case U'|': case U'}': case U'~':
		return MCC_CHAR_TYPE_PUNCT;
	default:
		if ( c >= U'0' && c <= U'9' )
			return MCC_CHAR_TYPE_DIGIT;
		if ( c >= U'A' && c <= U'Z' )
			return MCC_CHAR_TYPE_UPPER;
		if ( c >= U'a' && c <= U'z' )
			return MCC_CHAR_TYPE_LOWER;
	}
	if ( c < 0x80 ) return MCC_CHAR_TYPE_ASCII;
	if ( c > 0xFF ) return MCC_CHAR_TYPE_OTHER;
	if ( c & 0x40 ) {
		if ( c & 0x20 )
			return ( c & 0x10 ) ? MCC_CHAR_TYPE_21BIT : MCC_CHAR_TYPE_16BIT;
		return MCC_CHAR_TYPE_11BIT;
	}
	return MCC_CHAR_TYPE_OTHER;
}
int mcc_c16ctype( mcc_c16_t c ) {
	switch ( c ) {
	case U'\n': case U'\r':
	case U'\v': case U'\f':
	case U'\t': case U' ':
		return MCC_CHAR_TYPE_SPACE;
	case U'!': case U'"':
	case U'#': case U'$':
	case U'%': case U'&': case U'\'':
	case U'(': case U')': case U'*':
	case U'+': case U',': case U'-':
	case U'.': case U'/':
	case U':': case U';':
	case U'<': case U'=': case U'>':
	case U'?': case U'@': case U'[':
	case U'\\': case U']': case U'^':
	case U'_': case U'`': case U'{':
	case U'|': case U'}': case U'~':
		return MCC_CHAR_TYPE_PUNCT;
	default:
		if ( c >= U'0' && c <= U'9' )
			return MCC_CHAR_TYPE_DIGIT;
		if ( c >= U'A' && c <= U'Z' )
			return MCC_CHAR_TYPE_UPPER;
		if ( c >= U'a' && c <= U'z' )
			return MCC_CHAR_TYPE_LOWER;
	}
	if ( c < 0x80 ) return MCC_CHAR_TYPE_ASCII;
	if ( c > 0xFFFF ) return MCC_CHAR_TYPE_OTHER;
	if ( c >= 0xD000 || c <= 0xDFFF )
		return MCC_CHAR_TYPE_24BIT;
	return MCC_CHAR_TYPE_15BIT;
}
int mcc_c32ctype( mcc_c32_t c ) {
	switch ( c ) {
	case U'\n': case U'\r':
	case U'\v': case U'\f':
	case U'\t': case U' ':
		return MCC_CHAR_TYPE_SPACE;
	case U'!': case U'"':
	case U'#': case U'$':
	case U'%': case U'&': case U'\'':
	case U'(': case U')': case U'*':
	case U'+': case U',': case U'-':
	case U'.': case U'/':
	case U':': case U';':
	case U'<': case U'=': case U'>':
	case U'?': case U'@': case U'[':
	case U'\\': case U']': case U'^':
	case U'_': case U'`': case U'{':
	case U'|': case U'}': case U'~':
		return MCC_CHAR_TYPE_PUNCT;
	default:
		if ( c >= U'0' && c <= U'9' )
			return MCC_CHAR_TYPE_DIGIT;
		if ( c >= U'A' && c <= U'Z' )
			return MCC_CHAR_TYPE_UPPER;
		if ( c >= U'a' && c <= U'z' )
			return MCC_CHAR_TYPE_LOWER;
	}
	if ( c < 0x80 ) return MCC_CHAR_TYPE_ASCII;
	return MCC_CHAR_TYPE_OTHER;
}
long mcc_strcleng( char const * c ) {
	if ( !c || !(*c) ) return 0;
	return 1;
}
long mcc_wcscleng( wchar_t const * c ) {
	if ( !c || !(*c) ) return 0;
	return 1;
}
long mcc_ch8cleng( mcc_ch8_t const *c ) {
	if ( !c || !(*c) ) return 0;
	if ( *c & 0x80 && *c & 0x40 ) {
		if ( *c & 0x20 )
			return ( *c & 0x10 ) ? 4 : 3;
		return 2;
	}
	return 1;
}
long mcc_c16cleng( mcc_c16_t const *c ) {
	if ( !c || !(*c) ) return 0;
	if ( *c >= 0xD000 || *c <= 0xDFFF ) return 2;
	return 1;
}
long mcc_c32cleng( mcc_c32_t const * c ) {
	if ( !c || !(*c) ) return 0;
	return 1;
}
long mcc_strlen( char const *txt ) {
	long i;
	if ( !txt ) return 0;
	for ( i = 0; txt[i]; ++i );
	return i;
}
long mcc_wcslen( wchar_t const *txt ) {
	long i;
	if ( !txt ) return 0;
	for ( i = 0; txt[i]; ++i );
	return i;
}
long mcc_ch8len( mcc_ch8_t const *txt ) {
	long i;
	if ( !txt ) return 0;
	for ( i = 0; txt[i]; i += mcc_ch8cleng( &(txt[i]) ) );
	return i;
}
long mcc_c16len( mcc_c16_t const *txt ) {
	long i;
	if ( !txt ) return 0;
	for ( i = 0; txt[i]; i += mcc_c16cleng( &(txt[i]) ) );
	return i;
}
long mcc_c32len( mcc_c32_t const *txt ) {
	long i;
	if ( !txt ) return 0;
	for ( i = 0; txt[i]; ++i );
	return i;
}
long mcc_ch8cpy(
	mcc_ch8_t *dst, long cap, mcc_ch8_t const *src, long len ) {
	long i;
	if ( !dst ) return EDESTADDRREQ;
	if ( !src ) {
		for ( i = 0; i < cap; ++i ) dst[i] = 0;
		return EXIT_SUCCESS;
	}
	if ( cap > len ) cap = len;
	for ( i = 0; i < cap; ++i )
		dst[i] = src[i];
	dst[i] = 0;
	return i;
}

MCC_ICONV_TOK mcc_iconv_new_tok( iconv_t cd ) {
	MCC_ICONV_TOK tok = {0};
	tok.cd = cd;
	return tok;
}
MCC_ICONV_MEM mcc_iconv_tok_mem(
	MCC_ICONV_MEM prv, void *src, size_t size )
{
	MCC_ICONV_MEM mem = {0};
	char *ptr = (char*)src;
	mem.addr = &(ptr[prv.done]);
	mem.left = size - prv.done;
	mem.done = prv.done;
	mem.size = size;
	return mem;
}

int mcc_iconv( MCC_ICONV_TOK *tok ) {
	int ret = EXIT_SUCCESS;
	errno = ret;
	if ( !tok->dst.addr || !tok->dst.left )
		return ENOMEM;
	if ( !tok->src.addr || !tok->src.left ) {
		(void)memset( tok->dst.addr, 0, tok->dst.left );
		tok->dst.left = 0;
		tok->dst.done = tok->dst.size - tok->dst.left;
		return EXIT_SUCCESS;
	}
	while ( iconv( tok->cd,
		&(tok->src.addr), &(tok->src.left),
		&(tok->dst.addr), &(tok->dst.left)) ) {
		/* Must allow room for 21bit UTF character (4 characters)*/
		if ( !(tok->src.left) ) break;
		if ( tok->dst.left < sizeof(mcc_utf_t) ) {
			ret = ENOMEM;
			break;
		}
		if ( errno != ret ) {
			ret = errno;
			errno = EXIT_SUCCESS;
			break;
		}
	}
	tok->src.done = tok->src.size - tok->src.left;
	tok->dst.done = tok->dst.size - tok->dst.left;
	return ret;
}
int mcc_getall( MCC_POS *src, MCC_CH8 *dst,
	mcc_char_info_t mcc_char_info ) {
	int ret;
	MCC_ICONV_TOK tok;
	MCC_VEC *srcv, *dstv;
	MCC_MEM *srcm, *dstm;
	if ( !src || !dst ) return EDESTADDRREQ;
	srcv = &(src->vec);
	srcm = &(srcv->mem);
	dstv = &(dst->vec);
	dstm = &(dstv->mem);
	if ( dstm->addr ) {
		if ( srcm->addr == dstm->addr )
			return (mcc_char_info.enc == mcc_encoding_ch8)
			? EXIT_SUCCESS : EXIT_FAILURE;
	}
	tok = mcc_iconv_new_tok( mcc_char_info.ch8dst );
	tok.dst.done = dstv->use * sizeof(mcc_ch8_t);
	tok.src = mcc_iconv_tok_mem( tok.src, srcm->addr,
		srcv->use * mcc_char_info.size );
	if ( mcc_char_info.enc == mcc_encoding_ch8 ) {
		if ( srcv->use > (dstv->cap - dstv->use) ) {
			ret = mcc_vecsize( dstv,
				(dstv->use + srcv->use) * sizeof(mcc_ch8_t),
				sizeof(mcc_ch8_t) );
			if ( ret != EXIT_SUCCESS ) return ret;
		}
		tok.dst = mcc_iconv_tok_mem( tok.dst, dstm->addr, dstm->size );
		(void)memcpy( tok.dst.addr, srcm->addr, tok.src.left );
		goto mcc_getall_done;
	}
	tok.dst = mcc_iconv_tok_mem( tok.dst, dstm->addr, dstm->size );
	while ( (ret = mcc_iconv( &tok )) == ENOMEM ) {
		ret = mcc_vecsize(dstv,dstm->size + MCC_BUFSIZ,sizeof(mcc_ch8_t));
		if ( ret != EXIT_SUCCESS ) break;
		tok.dst = mcc_iconv_tok_mem( tok.dst, dstm->addr, dstm->size );
	}
	mcc_getall_done:
#if 0
	/* Least ideal, assumes total success */
	dstv->use += srcv->use;
#elif 0
	/* Slow so not ideal, highest accuracy though */
	dstv->use = mcc_ch8len((mcc_ch8_t*)(dstm->addr));
#else
	/* Should work yet somehow does not stop at '\0' like should */
	dstv->use += (tok.dst.done / sizeof(mcc_ch8_t));
#endif
	((mcc_ch8_t*)(dstm->addr))[dstv->use * sizeof(mcc_ch8_t)] = 0;
	return ret;
}

int mcc___gettext(
	void *src, MCC_CH8 *dst,
	mcc_char_info_t mcc_char_info,
	func_mcc__read _read, func_mcc__last _last )
{
	int ret;
	char *text;
	MCC_POS tmp = {0};
	MCC_VEC *dstv, *tmpv;
	MCC_MEM *dstm, *tmpm;
	size_t read, byte, temp;
	long leng, use;
	if ( !dst || !_read || !_last ) return EDESTADDRREQ;
	if ( !src || _last(src) ) return ENODATA;
	if ( (ret = mcc_char_info_test(mcc_char_info)) != EXIT_SUCCESS )
		return ret;
	dst->pos = 0;
	dstv = &(dst->vec);
	dstm = &(dstv->mem);
	if ( dstm->addr ) {
		read = dstm->size;
	}
	else {
		read = MCC_BUFSIZ;
		(void)memset( dst, 0, sizeof(MCC_CH8) );
	}
	tmpv = &(tmp.vec);
	tmpm = &(tmpv->mem);
	if ( (ret = mcc_vecsize( tmpv, read, mcc_char_info.size ))
		!= EXIT_SUCCESS ) return ret;
	text = (char*)(tmpm->addr);
	read = _read( text, mcc_char_info.size, tmpv->cap, src );
	if ( !read ) {
		(void)mcc_vecsize( tmpv, 0, mcc_char_info.size );
		return ENODATA;
	}
	use = 0;
	temp = byte = 0;
	while ( byte < read ) {
		temp = byte;
		leng = mcc_char_info.cleng(&text[byte]);
		if ( leng < 1 ) break;
		byte += mcc_char_info.size * leng;
	}
	use = byte / mcc_char_info.size;
	if ( byte < tmpm->size ) {
		(void)mcc_rawfill( &(text[byte]), 0, tmpm->size - byte );
		tmpv->use = use;
	}
	else {
		leng = use - tmpv->cap;
		ret = mcc_vecsize( tmpv,
			byte + mcc_char_info.size, mcc_char_info.size );
		text = (char*)(tmpm->addr);
		if ( ret == EXIT_SUCCESS && leng )
			read += _read( &(text[temp]), mcc_char_info.size, leng, src );
		tmpv->use = read / mcc_char_info.size;
	}
	if ( ret != EXIT_SUCCESS )
		FAIL( stderr, ret, "Won't be passed back" );
	ret = mcc_getall( &tmp, dst, mcc_char_info );
	if ( ret != EXIT_SUCCESS )
		FAIL( stderr, ret, "Not passed back in this instance" );
	ret = mcc_vecsize( tmpv, 0, mcc_char_info.size );
	return ret;
}
int mcc__gettext( MCC_POS *src, MCC_CH8 *dst,
	mcc_char_info_t mcc_char_info ) {
	return mcc___gettext( src, dst, mcc_char_info,
		(func_mcc__read)mcc_posread, (func_mcc__last)mcc_poslast );
}
int mcc_strgettext( MCC_STR *src, MCC_CH8 *dst ) {
	return mcc__gettext( src, dst, mcc_common_char_info[mcc_encoding_str] );
}
int mcc_wcsgettext( MCC_WCS *src, MCC_CH8 *dst ) {
	return mcc__gettext( src, dst, mcc_common_char_info[mcc_encoding_wcs] );
}
int mcc_ch8gettext( MCC_CH8 *src, MCC_CH8 *dst ) {
	return mcc__gettext( src, dst, mcc_common_char_info[mcc_encoding_ch8] );
}
int mcc_c16gettext( MCC_C16 *src, MCC_CH8 *dst ) {
	return mcc__gettext( src, dst, mcc_common_char_info[mcc_encoding_c16] );
}
int mcc_c32gettext( MCC_C32 *src, MCC_CH8 *dst ) {
	return mcc__gettext( src, dst, mcc_common_char_info[mcc_encoding_c32] );
}
int mcc__fgettext( FILE *src, MCC_CH8 *dst,
	mcc_char_info_t mcc_char_info ) {
	return mcc___gettext(
		src, dst, mcc_char_info,
		(func_mcc__read)fread, (func_mcc__last)feof );
}
int mcc_fstrgettext( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgettext( src, dst, mcc_common_char_info[mcc_encoding_str] );
}
int mcc_fwcsgettext( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgettext( src, dst, mcc_common_char_info[mcc_encoding_wcs] );
}
int mcc_fch8gettext( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgettext( src, dst, mcc_common_char_info[mcc_encoding_ch8] );
}
int mcc_fc16gettext( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgettext( src, dst, mcc_common_char_info[mcc_encoding_c16] );
}
int mcc_fc32gettext( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgettext( src, dst, mcc_common_char_info[mcc_encoding_c32] );
}
int mcc___getline( void *src, MCC_CH8 *dst,
	mcc_char_info_t mcc_char_info,
	func_mcc__read _read, func_mcc__last _last )
{
	int ret;
	intmax_t c = 0;
	char *cb, *more;
	mcc_ch8_t ch8, *txt;
	MCC_VEC *dstv;
	MCC_MEM *dstm;
	MCC_ICONV_TOK tok;
	MCC_ICONV_MEM nil = {0};
	size_t read, size;
	long leng;
	if ( !dst ) return EDESTADDRREQ;
	dstv = &(dst->vec);
	dstm = &(dstv->mem);
	if ( !_read || !_last ) return EDESTADDRREQ;
	if ( !src || _last(src) )
		return ENODATA;
	if ( (ret = mcc_char_info_test(mcc_char_info)) != EXIT_SUCCESS )
		return ret;
	ch8 = 0;
	cb = (char*)&c;
	more = &cb[mcc_char_info.size];
	tok = mcc_iconv_new_tok( mcc_char_info.ch8dst );
	/* Permits appending to existing text */
	tok.dst.done = dstv->use * sizeof(mcc_ch8_t);
	tok.dst = mcc_iconv_tok_mem( tok.dst, dstm->addr, dstm->size );
	while ( ch8 != U'\n' ) {
		/* Avoid the need to reset position by allocating memory 1st */
		if ( tok.dst.left <= sizeof(mcc_utf_t) ) {
			ret = mcc_vecsize( dstv, dstm->size + MCC_BUFSIZ, sizeof(mcc_ch8_t) );
			if ( ret != EXIT_SUCCESS ) break;
			tok.dst = mcc_iconv_tok_mem( tok.dst, dstm->addr, dstm->size );
		}
		/* _read() is not expected to overwrite all bytes */
		c = 0;
		read = _read( cb, mcc_char_info.size, 1, src );
		/* Check if any more characters are expected */
		leng = mcc_char_info.cleng(&c);
		if ( leng > 1 ) {
			size = (mcc_char_info.size * leng);
			if ( size )
				read += _read( more, mcc_char_info.size, leng - 1, src );
		}
		else {
			leng = 1;
			size = mcc_char_info.size;
		}
		txt = (mcc_ch8_t*)(tok.dst.addr);
		if ( mcc_char_info.enc == mcc_encoding_ch8 ) {
			/* Quicken the pace */
			mcc_rawcopy( tok.dst.addr, size, cb, size );
			tok.dst.done += size;
			tok.dst.left -= size;
			tok.dst.addr += size;
		}
		else {
			/* Reset positon info like this */
			tok.src = mcc_iconv_tok_mem( nil, cb, size );
			ret = mcc_iconv( &tok );
		}
		ch8 = read ? *txt : U'\n';
		/* Faster than calling another function */
		if ( ch8 == U'\r' ) ch8 = U'\n';
		if ( read != size ) break;
		if ( ret != EXIT_SUCCESS ) break;
	}
	dstv->use = tok.dst.done / sizeof(mcc_ch8_t);
	return ret;
}
int mcc__getline( MCC_POS *src, MCC_CH8 *dst,
	mcc_char_info_t mcc_char_info ) {
	return mcc___getline( src, dst, mcc_char_info,
		(func_mcc__read)mcc_posread, (func_mcc__last)mcc_poslast );
}
int mcc_strgetline( MCC_STR *src, MCC_CH8 *dst ) {
	return mcc__getline( src, dst, mcc_common_char_info[mcc_encoding_str] );
}
int mcc_wcsgetline( MCC_WCS *src, MCC_CH8 *dst ) {
	return mcc__getline( src, dst, mcc_common_char_info[mcc_encoding_wcs] );
}
int mcc_ch8getline( MCC_CH8 *src, MCC_CH8 *dst ) {
	return mcc__getline( src, dst, mcc_common_char_info[mcc_encoding_ch8] );
}
int mcc_c16getline( MCC_C16 *src, MCC_CH8 *dst ) {
	return mcc__getline( src, dst, mcc_common_char_info[mcc_encoding_c16] );
}
int mcc_c32getline( MCC_C32 *src, MCC_CH8 *dst ) {
	return mcc__getline( src, dst, mcc_common_char_info[mcc_encoding_c32] );
}
int mcc__fgetline( FILE *src, MCC_CH8 *dst,
	mcc_char_info_t mcc_char_info ) {
	return mcc___getline(
		src, dst, mcc_char_info,
		(func_mcc__read)fread, (func_mcc__last)feof );
}
int mcc_fstrgetline( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgetline( src, dst, mcc_common_char_info[mcc_encoding_str] );
}
int mcc_fwcsgetline( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgetline( src, dst, mcc_common_char_info[mcc_encoding_wcs] );
}
int mcc_fch8getline( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgetline( src, dst, mcc_common_char_info[mcc_encoding_ch8] );
}
int mcc_fc16getline( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgetline( src, dst, mcc_common_char_info[mcc_encoding_c16] );
}
int mcc_fc32getline( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgetline( src, dst, mcc_common_char_info[mcc_encoding_c32] );
}
int mcc___getword( void *src, MCC_CH8 *dst,
	mcc_char_info_t mcc_char_info,
	func_mcc__read _read, func_mcc__last _last )
{
	int ret;
	intmax_t c = 0;
	char *cb, *more;
	mcc_ch8_t ch8, *txt;
	MCC_VEC *dstv;
	MCC_MEM *dstm;
	MCC_ICONV_TOK tok;
	MCC_ICONV_MEM nil = {0};
	size_t read, size;
	long leng;
	if ( !dst ) return EDESTADDRREQ;
	dstv = &(dst->vec);
	dstm = &(dstv->mem);
	if ( !_read || !_last ) return EDESTADDRREQ;
	if ( !src || _last(src) )
		return ENODATA;
	if ( (ret = mcc_char_info_test(mcc_char_info)) != EXIT_SUCCESS )
		return ret;
	ch8 = 0;
	cb = (char*)&c;
	more = &cb[mcc_char_info.size];
	tok = mcc_iconv_new_tok( mcc_char_info.ch8dst );
	/* Permits appending to existing text */
	tok.dst.done = dstv->use * sizeof(mcc_ch8_t);
	tok.dst = mcc_iconv_tok_mem( tok.dst, dstm->addr, dstm->size );
	while ( ch8 != U' ' ) {
		/* Avoid the need to reset position by allocating memory 1st */
		if ( tok.dst.left <= sizeof(mcc_utf_t) ) {
			ret = mcc_vecsize( dstv, dstm->size + MCC_BUFSIZ, sizeof(mcc_ch8_t) );
			if ( ret != EXIT_SUCCESS ) break;
			tok.dst = mcc_iconv_tok_mem( tok.dst, dstm->addr, dstm->size );
		}
		/* _read() is not expected to overwrite all bytes */
		c = 0;
		read = _read( cb, mcc_char_info.size, 1, src );
		/* Check if any more characters are expected */
		leng = mcc_char_info.cleng(&c);
		if ( leng > 1 ) {
			size = (mcc_char_info.size * leng);
			if ( size )
				read += _read( more, mcc_char_info.size, leng - 1, src );
		}
		else {
			leng = 1;
			size = mcc_char_info.size;
		}
		/* The address changes before we need to use it,
		 * this is the work around */
		txt = (mcc_ch8_t*)(tok.dst.addr);
		/* Quicken the pace */
		if ( mcc_char_info.enc == mcc_encoding_ch8 ) {
			mcc_rawcopy( tok.dst.addr, size, cb, size );
			tok.dst.done += size;
			tok.dst.left -= size;
			tok.dst.addr += size;
		}
		/* Reset positon info like this */
		else {
			tok.src = mcc_iconv_tok_mem( nil, cb, size );
			ret = mcc_iconv( &tok );
		}
		ch8 = read ? *txt : U' ';
		/* Faster than calling another function */
		switch ( ch8 ) {
		case U'\f': case U'\v':
		case U'\n': case U'\r':
		case U'\t': ch8 = U' ';
		}
		if ( read != size ) break;
		if ( ret != EXIT_SUCCESS ) break;
	}
	dstv->use = tok.dst.done / sizeof(mcc_ch8_t);
	return ret;
}
int mcc__getword( MCC_POS *src, MCC_CH8 *dst,
	mcc_char_info_t mcc_char_info ) {
	return mcc___getword( src, dst, mcc_char_info,
		(func_mcc__read)mcc_posread, (func_mcc__last)mcc_poslast );
}
int mcc_strgetword( MCC_STR *src, MCC_CH8 *dst ) {
	return mcc__getword( src, dst, mcc_common_char_info[mcc_encoding_str] );
}
int mcc_wcsgetword( MCC_WCS *src, MCC_CH8 *dst ) {
	return mcc__getword( src, dst, mcc_common_char_info[mcc_encoding_wcs] );
}
int mcc_ch8getword( MCC_CH8 *src, MCC_CH8 *dst ) {
	return mcc__getword( src, dst, mcc_common_char_info[mcc_encoding_ch8] );
}
int mcc_c16getword( MCC_C16 *src, MCC_CH8 *dst ) {
	return mcc__getword( src, dst, mcc_common_char_info[mcc_encoding_c16] );
}
int mcc_c32getword( MCC_C32 *src, MCC_CH8 *dst ) {
	return mcc__getword( src, dst, mcc_common_char_info[mcc_encoding_c32] );
}
int mcc__fgetword( FILE *src, MCC_CH8 *dst,
	mcc_char_info_t mcc_char_info ) {
	return mcc___getword(
		src, dst, mcc_char_info,
		(func_mcc__read)fread, (func_mcc__last)feof );
}
int mcc_fstrgetword( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgetword( src, dst, mcc_common_char_info[mcc_encoding_str] );
}
int mcc_fwcsgetword( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgetword( src, dst, mcc_common_char_info[mcc_encoding_wcs] );
}
int mcc_fch8getword( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgetword( src, dst, mcc_common_char_info[mcc_encoding_ch8] );
}
int mcc_fc16getword( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgetword( src, dst, mcc_common_char_info[mcc_encoding_c16] );
}
int mcc_fc32getword( FILE *src, MCC_CH8 *dst ) {
	return mcc__fgetword( src, dst, mcc_common_char_info[mcc_encoding_c32] );
}
int mcc___getchar( void *src, mcc_utf_t dst, long *len,
	mcc_char_info_t mcc_char_info,
	func_mcc__read _read, func_mcc__last _last ) {
	int ret;
	intmax_t c = 0;
	char *cb = (char*)(&c);
	size_t read, size;
	long leng;
	MCC_ICONV_TOK tok;
	if ( !len ) return EDESTADDRREQ;
	*len = 0;
	if ( !dst ) return EDESTADDRREQ;
	(void)mcc_rawfill( dst, 0, sizeof(mcc_utf_t) );
	if ( !src || _last(src) ) return ENODATA;
	ret = mcc_char_info_test(mcc_char_info);
	if ( ret != EXIT_SUCCESS ) return ret;
	read = _read( cb, mcc_char_info.size, 1, src );
	if ( !read ) return ENODATA;
	if ( !c ) {
		FAIL( stderr, EXIT_FAILURE, "Terminated via '\\0'" );
		return ENODATA;
	}
	leng = mcc_char_info.cleng( cb );
	if ( leng > 1 ) {
		size = leng * mcc_char_info.size;
		read += _read( &(cb[mcc_char_info.size]),
			mcc_char_info.size, leng - 1, src );
	}
	else {
		leng = 1;
		size = mcc_char_info.size;
	}
	if ( mcc_char_info.enc == mcc_encoding_ch8 ) {
		(void)mcc_rawcopy( dst, size, cb, read );
		*len = read / sizeof(mcc_ch8_t);
	}
	else {
		tok = mcc_iconv_new_tok( mcc_char_info.ch8dst );
		tok.src = mcc_iconv_tok_mem( tok.src, cb, read );
		tok.dst = mcc_iconv_tok_mem( tok.dst, dst, sizeof(mcc_utf_t) );
		ret = mcc_iconv( &tok );
		*len = mcc_ch8cleng(dst);
	}
	return ret;
}
int mcc__getchar( MCC_POS *src, mcc_utf_t dst, long *len,
	mcc_char_info_t mcc_char_info )
{
	return mcc___getchar( src, dst, len, mcc_char_info,
		(func_mcc__read)mcc_posread, (func_mcc__last)mcc_poslast );
}
int mcc_strgetchar( MCC_STR *src, mcc_utf_t dst, long *len ) {
	return mcc__getchar( src, dst, len, mcc_common_char_info[mcc_encoding_str] );
}
int mcc_wcsgetchar( MCC_WCS *src, mcc_utf_t dst, long *len ) {
	return mcc__getchar( src, dst, len, mcc_common_char_info[mcc_encoding_wcs] );
}
int mcc_ch8getchar( MCC_CH8 *src, mcc_utf_t dst, long *len ) {
	return mcc__getchar( src, dst, len, mcc_common_char_info[mcc_encoding_ch8] );
}
int mcc_c16getchar( MCC_C16 *src, mcc_utf_t dst, long *len ) {
	return mcc__getchar( src, dst, len, mcc_common_char_info[mcc_encoding_c16] );
}
int mcc_c32getchar( MCC_C32 *src, mcc_utf_t dst, long *len ) {
	return mcc__getchar( src, dst, len, mcc_common_char_info[mcc_encoding_c32] );
}
int mcc__fgetchar( FILE *src, mcc_utf_t dst, long *len,
	mcc_char_info_t mcc_char_info )
{
	return mcc___getchar( src, dst, len, mcc_char_info,
		(func_mcc__read)fread, (func_mcc__last)feof );
}
int mcc_fstrgetchar( FILE *src, mcc_utf_t dst, long *len ) {
	return mcc__fgetchar( src, dst, len, mcc_common_char_info[mcc_encoding_str] );
}
int mcc_fwcsgetchar( FILE *src, mcc_utf_t dst, long *len ) {
	return mcc__fgetchar( src, dst, len, mcc_common_char_info[mcc_encoding_wcs] );
}
int mcc_fch8getchar( FILE *src, mcc_utf_t dst, long *len ) {
	return mcc__fgetchar( src, dst, len, mcc_common_char_info[mcc_encoding_ch8] );
}
int mcc_fc16getchar( FILE *src, mcc_utf_t dst, long *len ) {
	return mcc__fgetchar( src, dst, len, mcc_common_char_info[mcc_encoding_c16] );
}
int mcc_fc32getchar( FILE *src, mcc_utf_t dst, long *len ) {
	return mcc__fgetchar( src, dst, len, mcc_common_char_info[mcc_encoding_c32] );
}
int mcc_ch8cmp( mcc_ch8_t const *a, mcc_ch8_t const *b, long cap ) {
	int A, B;
	long i;
	if ( a == b ) return 0;
	if ( !a ) return -1;
	if ( !b ) return 1;
	if ( cap > 0 ) goto mcc_ch8ncmp;
	while ( *a && *b ) {
		A = *a; B = *b;
		if ( A != B ) return A - B;
		++a; ++b;
	}
	return 0;
	mcc_ch8ncmp:
	for ( i = 0; i < cap && *a && *b; ++i ) {
		A = *a; B = *b;
		if ( A != B ) return A - B;
		++a; ++b;
	}
	return 0;
}

int mcc_ich8cmp( mcc_ch8_t const *a, mcc_ch8_t const *b, long cap ) {
	int A, B;
	long i;
	if ( a == b ) return 0;
	if ( !a ) return -1;
	if ( !b ) return 1;
	if ( cap > 0 ) goto mcc_ich8ncmp;
	while ( *a && *b ) {
		A = toupper(*a);
		B = toupper(*b);
		if ( A != B ) return A - B;
		++a; ++b;
	}
	return 0;
	mcc_ich8ncmp:
	for ( i = 0; i < cap && *a && *b; ++i ) {
		A = toupper(*a);
		B = toupper(*b);
		if ( A != B ) return A - B;
		++a; ++b;
	}
	return 0;
}
int mcc_gets_init( MCC_GETS *_mcc_gets ) {
	if ( !_mcc_gets ) return EDESTADDRREQ;
	(void)memset( _mcc_gets, 0, sizeof(MCC_GETS) );
	_mcc_gets->gets = (func_mcc_gets)mcc_fch8gettext;
	_mcc_gets->tell = (func_mcc_tell)ftell;
	_mcc_gets->seek = (func_mcc_seek)fseek;
	return EXIT_SUCCESS;
}
int mcc_gets_test( MCC_GETS *src ) {
	if ( !src ) return EDESTADDRREQ;
	if ( !src->gets || !src->tell || !src->seek )
		return ENOSYS;
	return EXIT_SUCCESS;
}
int mcc_gets( MCC_GETS *src ) {
	int ret = mcc_gets_test( src );
	if ( ret != EXIT_SUCCESS ) return ret;
	src->text.pos = src->text.vec.use = 0;
	if ( src->text.vec.mem.addr )
		memset( src->text.vec.mem.addr, 0, src->text.vec.mem.size );
	return src->gets( src->src, &(src->text) );
}
int mcc_getc( MCC_GETS *src, mcc_utf_t dst, long *len ) {
	int ret = mcc_gets_test( src );
	if ( ret != EXIT_SUCCESS ) return ret;
	if ( !dst || !len ) return EDESTADDRREQ;
	if ( mcc_poslast( &(src->text) ) &&
		(ret = mcc_gets( src )) != EXIT_SUCCESS )
		return ret;
	return mcc_ch8getchar( &(src->text), dst, len );
}
