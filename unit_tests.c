#include "tsc.h"
#include "next.h"
#include "base62.h"

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

int append_data( void const *data, size_t size, size_t *pos ) {
	int ret;
	char null = 0;
	size_t zero = 1;
	if ( !gdata.leng ) {
		if ( (ret = add2tab( &null, zero, NULL )) != EXIT_SUCCESS )
			return ret;
		if ( (ret = add2tab( &zero, sizeof(size_t), NULL )) != EXIT_SUCCESS )
			return ret;
	}
	if ( (ret = add2tab( data, size, pos )) != EXIT_SUCCESS )
			return ret;
	if ( (ret = add2tab( &size, sizeof(size_t), NULL )) != EXIT_SUCCESS )
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
char8_t *type_names[TYPES_UPTO] = { u8"?", u8"*",
	u8"signed char", u8"signed short", u8"signed int", u8"signed long", u8"signed long long",
	u8"unsigned char", u8"unsigned short", u8"unsigned int", u8"unsigned long", u8"unsigned long long",
	u8"char", u8"char8_t", u8"char16_t", u8"char32_t", u8"wchar_t",
NULL };

int literal( NEXTC *_nextc, DATA *dst ) {
	int ret = nextc_validate(_nextc);
	DATA _data = {0};
	size_t invalid, type;
	_Bool readmore = 1;
	if ( ret ) goto literal_done;
	ret = match2_data( u8"?", sizeof(u8"?"), &invalid );
	if ( ret ) goto literal_done;
	type = invalid;
	switch ( _nextc->c[0] ) {
	case 'U':
		ret = match2_data( u8"char32_t", sizeof(u8"char32_t"), &type );
		if ( ret ) goto literal_done;
		break;
	case 'u':
		readmore = 0;
		if ( !nextc(_nextc) ) {
			ret =
	}
	literal_done:
	if ( dst ) *dst = _data;
	return ret;
}

int main() {
	int ret;
	size_t i;
	char8_t *name;
	for ( i = 0; i < TYPES_UPTO; ++i ) {
		if ( !(name = type_names[i]) ||
		(ret = append_data(name,utflen(name)+1,&types[i])) )
			break;
	}
	if (ret) FAIL( stderr, ret, "" );
	if ( gdata ) free(gdata);
	return ret ? 1 : 0;
}
