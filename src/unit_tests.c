#include "mcc_tsc.h"
#include "mcc_get.h"
#include "mcc_base62.h"

typedef struct _tab {
	size_t have;
	size_t size;
	size_t upto;
	size_t leng;
	char * data;
} TAB;

TAB gdata = {0}, gsize = {0};

int add2tab( TAB *tab, void const *data, size_t size, size_t *pos ) {
	int ret;
	TAB _tab;
	if ( !tab || !data ) return EDESTADDRREQ;
	_tab = *tab;
	for ( ; _tab.have < size; _tab.have += BUFSIZ, _tab.size += BUFSIZ );
	if ( _tab.size > tab->size ) {
		_tab.data = (char*)realloc( _tab.data, size );
		if ( !_tab.data ) {
			ret = errno;
			errno = 0;
			return ret;
		}
	}
	(void)memcpy( &_tab.data[_tab.leng], data, size );
	if ( pos ) *pos = _tab.leng;
	_tab.leng += size;
	_tab.have = _tab.size - _tab.leng;
	_tab.upto++;
	*tab = _tab;
	return EXIT_SUCCESS;
}

int append_data( TAB *tab, void const *data, size_t size, size_t *pos ) {
	int ret;
	char null = 0;
	size_t zero = 1;
	if ( !gdata.leng ) {
		if ( (ret = add2tab( tab, &null, zero, NULL )) != EXIT_SUCCESS )
			return ret;
		if ( (ret = add2tab( tab, &zero, sizeof(size_t), NULL )) != EXIT_SUCCESS )
			return ret;
	}
	if ( (ret = add2tab( tab, data, size, pos )) != EXIT_SUCCESS )
			return ret;
	if ( (ret = add2tab( tab, &size, sizeof(size_t), NULL )) != EXIT_SUCCESS )
		return ret;
	return EXIT_SUCCESS;
}
int match2_data( void const *data, size_t size, size_t *pos ) {
	size_t i, used;
	if ( gdata.leng != gsize.leng ) return ENOMEM;
	for ( i = 0; i < gdata.leng; i += used ) {
		used = ((size_t*)gsize.data)[i];
		if ( used != size ) continue;
		if ( memcmp( data, &gdata.data[i], size ) == 0 ) {
			if ( pos ) *pos = i;
			return EXIT_SUCCESS;
		}
	}
	if ( pos ) *pos = -1;
	return ENODATA;
}

typedef struct _data {
	size_t type;
	size_t name;
	size_t size;
	void * data;
} DATA;

#define TYPES_UPTO 50
size_t types[TYPES_UPTO] = {0};
mcc_ch8_t *type_names[TYPES_UPTO] = { u8"?", u8"*",
	u8"signed char", u8"signed short", u8"signed int", u8"signed long", u8"signed long long",
	u8"unsigned char", u8"unsigned short", u8"unsigned int", u8"unsigned long", u8"unsigned long long",
	u8"char", u8"mcc_ch8_t", u8"char16_t", u8"char32_t", u8"wchar_t",
NULL };

int literal( MCC_GETC *src, DATA *dst ) {
	int ret = mcc_getc_validate(src);
	DATA _data = {0};
	size_t invalid, type;
	_Bool readmore = 1;
	if ( ret ) goto literal_done;
	ret = match2_data( u8"?", sizeof(u8"?"), &invalid );
	if ( ret ) goto literal_done;
	type = invalid;
	switch ( src->c[0] ) {
	case 'U':
		ret = match2_data( u8"char32_t", sizeof(u8"char32_t"), &type );
		if ( ret ) goto literal_done;
		break;
	case 'u':
		readmore = 0;
		if ( mcc_getc( src ) )
			ret = ENODATA;
	}
	literal_done:
	if ( readmore ) mcc_getc( src );
	if ( dst ) *dst = _data;
	return ret;
}

typedef struct symbol {
	long type;
	long name;
	long leng;
	long data;
	long size;
} SYMBOL;

MCC_CH8 deftab = {0}, strtab = {0};
MCC_VEC symtab = {0};

mcc_ch8_t *defines[] = {
	u8"#define CHAR_BIT 8",
	u8"#define SHRT_BIT (CHAR_BIT * 8)",
	NULL
};

int readname( MCC_GETC *src, MCC_CH8 *dst ) {
	int ret, type;
	mcc_ch8_t *text;
	long i = 0;
	_Bool stdc_compliance = 0;
	// We'll use C89 for time being since still learning assembler
	// long stdc_version = 198900L;
	if ( !dst ) return EDESTADDRREQ;
	ret = mcc_getc_validate(src);
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = mcc_vecsize( &(dst->vec), MCC_BUFSIZ, sizeof(mcc_ch8_t) );
	if ( ret != EXIT_SUCCESS ) return ret;
	text = (mcc_ch8_t*)(dst->vec.mem.addr);
	(void)memset( text, 0, dst->vec.mem.size );
	dst->vec.use = 0;
	do {
		type = mcc_ch8ctype( src->c[0] );
		if ( type < MCC_CHAR_TYPE_SCORE ||
			type > MCC_CHAR_TYPE_OTHER ) break;
		if ( i == 0 && type == MCC_CHAR_TYPE_DIGIT ) {
			ret = EILSEQ;
			break;
		}
		if ( stdc_compliance && type > MCC_CHAR_TYPE_DIGIT ) {
			ret = EILSEQ;
			break;
		}
		if ( i + src->len > dst->vec.cap ) {
			ret = mcc_vecsize( &(dst->vec),
				dst->vec.mem.size + MCC_BUFSIZ, sizeof(mcc_ch8_t) );
			if ( ret != EXIT_SUCCESS ) break;
			text = (mcc_ch8_t*)dst->vec.mem.addr;
		}
		memcpy( &(text[i]), src->c, src->len );
		i += src->len;
	} while ( (ret = mcc_getc( src )) == EXIT_SUCCESS );
	return ret;
}

int value( MCC_GETC *src, SYMBOL *dst ) {
	int ret;
	MCC_CH8 name = {0};
	//mcc_ch8_t *text, *names = deftab.vec.mem.addr;
	//long i;
	if ( !dst ) return EDESTADDRREQ;
	(void)memset( dst, 0, sizeof(SYMBOL) );
	ret = mcc_getc_validate(src);
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = mcc_vecsize( &(name.vec), MCC_BUFSIZ, sizeof(mcc_ch8_t) );
	if ( ret != EXIT_SUCCESS ) return ret;
	ret = readname( src, &name );
	if ( ret != EXIT_SUCCESS ) return ret;
	//text = (mcc_ch8_t*)(name.vec.mem.addr);
	return ret;
}

int define( MCC_GETC *src ) {
	int ret = mcc_getc_validate(src);
	return ret;
}

int main() {
	int ret = EXIT_SUCCESS;
	long i;
	MCC_GETS _mcc_gets = {0};
	MCC_GETC _mcc_getc = {0};
	MCC_CH8 utf;
	_mcc_getc.src = &_mcc_gets;
	_mcc_gets.src = &utf;
	_mcc_gets.tell = (func_mcc_tell)mcc_ch8tell;
	_mcc_gets.seek = (func_mcc_seek)mcc_ch8seek;
	_mcc_gets.gets = (func_mcc_gets)mcc_ch8gettext;
	for ( i = 0; defines[i]; ++i ) {
		(void)memset(&utf,0,sizeof(MCC_CH8));
		utf.vec.mem.addr = defines[i];
		utf.vec.use = mcc_ch8len( defines[i] );
		utf.vec.mem.size = utf.vec.cap = utf.vec.use + 1;
		ret = define(&_mcc_getc);
		if ( ret != EXIT_SUCCESS ) break;
	}
	return (ret == EXIT_SUCCESS) ? EXIT_SUCCESS : EXIT_FAILURE;
}
