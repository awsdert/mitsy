#include "mcc_mem.h"

int mcc_set_int_max( void *dst, size_t bits ) {
	uchar *d;
	size_t i, size;
	if ( !dst || !bits ) return EDESTADDRREQ;
	d = (uchar*)dst;
	size = bits / CHAR_BIT;
	for ( i = 0; i < size; ++i )
		d[i] = -1;
	size %= CHAR_BIT;
	if ( !size ) return EXIT_SUCCESS;
	d[i] = ~((~0u) << (--size));
	return EXIT_SUCCESS;
}
int mcc_set_int_min( void *dst, size_t bits ) {
	uchar *d;
	size_t i, size;
	if ( !dst || !bits ) return EDESTADDRREQ;
	d = (uchar*)dst;
	size = bits / CHAR_BIT;
	for ( i = 0; i < size; ++i )
		d[i] = 0u;
	size %= CHAR_BIT;
	if ( !size ) return EXIT_SUCCESS;
	d[i] = ((~0u) << (--size));
	return EXIT_SUCCESS;
}
int mcc_set_uint_max( void *dst, size_t bits ) {
	uchar *d;
	size_t i, size;
	if ( !dst || !bits ) return EDESTADDRREQ;
	d = (uchar*)dst;
	size = bits / CHAR_BIT;
	for ( i = 0; i < size; ++i )
		d[i] = -1;
	size %= CHAR_BIT;
	if ( !size ) return EXIT_SUCCESS;
	d[i] = -1;
	return EXIT_SUCCESS;
}

int mcc_memsize( MCC_MEM *mcc_mem, size_t size ) {
	int ret = EXIT_SUCCESS;
	char *addr;
	if ( !mcc_mem ) {
		ret = EDESTADDRREQ;
		FAIL( stderr, ret, "" );
		return ret;
	}
	if ( mcc_mem->size == size ) return EXIT_SUCCESS;
	addr = mcc_mem->addr;
	errno = EXIT_SUCCESS;
	if ( size ) {
		if ( addr )	addr = realloc( addr, size );
		else addr = malloc( size );
		ret = errno;
		if ( ret != (errno = EXIT_SUCCESS) ) {
			FAIL( stderr, ret, "" );
			return ret;
		}
		mcc_mem->addr = addr;
		if ( mcc_mem->size < size )
			(void)memset( &(addr[mcc_mem->size]), 0, size - mcc_mem->size );
		mcc_mem->size = size;
	}
	else if ( addr )
		free( addr );
	return ret;
}
int mcc_rawcomp(
	void const *a, size_t a_size,
	void const *b, size_t b_size ) {
	schar const *A, *B;
	size_t i, size;
	if ( a == b ) return 0;
	if ( !a || !a_size ) return (b && b_size) ? -1 : 0;
	if ( !b || !b_size ) return 1;
	A = (schar const*)a;
	B = (schar const*)b;
	size = (b_size > a_size) ? a_size : b_size;
	for ( i = 0; i < size; ++i, ++A, ++B ) {
		if ( *A != *B ) return *A - *B;
	}
	for ( ; i < a_size; ++i, ++A ) {
		if ( *A ) return 1;
	}
	for ( ; i < b_size; ++i, ++B ) {
		if ( *B ) return -1;
	}
	return 0;
}
size_t mcc_rawcopy(
	void *dst, size_t dst_size,
	void const *src, size_t src_size ) {
	size_t i, size;
	uchar *d;
	uchar const *s;
	if ( !dst || !dst_size ) {
		FAIL( stderr, EDESTADDRREQ, "" );
		return 0;
	}
	if ( dst == src ) return dst_size;
	if ( !src || !src_size ) return 0;
	d = (uchar*)dst;
	s = (uchar const *)src;
	size = (dst_size < src_size) ? dst_size : src_size;
	for ( i = 0; i < size; ++i, ++d, ++s ) *d = *s;
	return i;
}

int mcc_vecsize( MCC_VEC *src, size_t size, size_t pern ) {
	int ret;
	size_t cap;
	if ( !src ) {
		ret = EDESTADDRREQ;
		FAIL( stderr, ret, "" );
		return ret;
	}
	if ( size == src->mem.size ) return EXIT_SUCCESS;
	cap = pern ? (size / pern) : 0;
	if ( cap > LONG_MAX ) return E2BIG;
	ret = mcc_memsize( &(src->mem), size );
	if ( ret == EXIT_SUCCESS ) {
		src->cap = cap;
		if ( src->use > src->cap ) src->use = src->cap;
	}
	return ret;
}
void* mcc_vecnode( MCC_VEC *mcc_vec, long pos, size_t pern ) {
	int ret = mcc_vecclamp( mcc_vec );
	size_t byte;
	char *buff;
	if ( ret != EXIT_SUCCESS || pos >= mcc_vec->use )
		return NULL;
	buff = (char*)(mcc_vec->mem.addr);
	byte = pos * pern;
	return &(buff[byte]);
}

int mcc_vecclamp( MCC_VEC *src ) {
	if ( !src ) return EDESTADDRREQ;
	if ( !(src->mem.addr) ) {
		(void)memset( src, 0, sizeof(MCC_VEC) );
		return ENOMEM;
	}
	if ( src->cap < 0 ) src->cap = 0;
	if ( src->use > src->cap ) src->use = src->cap;
	return EXIT_SUCCESS;
}

int mcc_posclamp( MCC_POS *src ) {
	if ( !src ) return EDESTADDRREQ;
	if ( !src->vec.mem.addr ) {
		(void)memset( src, 0, sizeof(MCC_POS) );
		return ENOMEM;
	}
	(void)mcc_vecclamp( &(src->vec) );
	if ( src->pos < 0 ) src->pos = 0;
	if ( src->pos > src->vec.use )
		src->pos = src->vec.use;
	return EXIT_SUCCESS;
}

int mcc_poslast( MCC_POS *src ) {
	if ( mcc_posclamp( src ) != EXIT_SUCCESS ) return 1;
	return ( src->pos >= src->vec.use ) ? 1 : 0;
}

void* mcc_posnode( MCC_POS *src, size_t pern ) {
	int ret = mcc_posclamp( src );
	if ( ret != EXIT_SUCCESS ) return NULL;
	return mcc_vecnode( &(src->vec), src->pos, pern );
}

long mcc_postell( MCC_POS *mcc_pos ) {
	if ( !mcc_pos ) return -1;
	mcc_posclamp(mcc_pos);
	return mcc_pos->pos;
}
int mcc_posseek( MCC_POS *mcc_pos, long pos, int from ) {
	int ret;
	if ( !mcc_pos ) {
		ret = EDESTADDRREQ;
		FAIL( stderr, ret, "" );
		return ret;
	}
	switch ( from ) {
	case SEEK_SET: mcc_pos->pos = pos; break;
	case SEEK_CUR: mcc_pos->pos += pos; break;
	case SEEK_END: mcc_pos->pos = mcc_pos->vec.use + pos; break;
	default:
		return EXIT_FAILURE;
	}
	mcc_posclamp(mcc_pos);
	return EXIT_SUCCESS;
}

size_t mcc_posread(
	void *dst, size_t pern, size_t want, MCC_POS *src ) {
	size_t read, byte, left;
	MCC_VEC *srcv;
	MCC_MEM *srcm;
	if ( !dst ) {
		FAIL( stderr, EDESTADDRREQ, "" );
		return 0;
	}
	if ( !want || !pern ) return 0;
	read = pern * want;
	if ( mcc_poslast(src) )
		goto mcc_posread_none;
	srcv = &(src->vec);
	srcm = &(srcv->mem);
	byte = src->pos * pern;
	if ( byte > srcm->size || !(srcm->addr) )
		goto mcc_posread_none;
	left = srcm->size - byte;
	if ( read > left ) {
		read = left;
		want = left / pern;
	}
	read = mcc_rawcopy( dst, read, &(((char*)(srcm->addr))[byte]), read );
	src->pos += want;
	return read;
	mcc_posread_none:
	(void)memset( dst, 0, read );
	return 0;
}
int mcc_rawfill( void *dst, char c, size_t size ) {
	char *d = dst;
	if ( !d ) return EDESTADDRREQ;
	for ( ; size--; ++d ) *d = c;
	return EXIT_SUCCESS;
}
