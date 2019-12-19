#include "mcc_base62.h"
#define COPY_TEST_TEXT 0
MCC_GETS setup_test( MCC_POS *tmp,
	void *text, size_t size, size_t csize,
	char *label, func_mcc_gets _gets ) {
#if COPY_TEST_TEXT
	int ret;
#endif
	MCC_GETS src = {0};
	MCC_VEC *tmpv = &(tmp->vec);
	MCC_MEM *tmpm = &(tmpv->mem);
	mcc_gets_init(&src);
	src.src = tmp;
	src.gets = _gets;
	src.tell = (func_mcc_tell)mcc_postell;
	src.seek = (func_mcc_seek)mcc_posseek;
#if COPY_TEST_TEXT
	if ( (ret = mcc_vecsize( tmpv, size, csize )) != EXIT_SUCCESS )
		FAIL( stderr, ret, "" );
	if ( mcc_rawcopy( tmpm->addr, size, text, size ) != size )
		FAIL( stderr, ERANGE, "" );
	tmpv->use = (size / csize);
#else
	tmpm->addr = text;
	tmpm->size = size;
	tmpv->cap = size / csize;
	tmpv->use = tmpv->cap;
#endif
	if ( tmpv->use ) tmpv->use--;
	printf("%s:",label);
	return src;
}

int clean_test( int ret, size_t csize, MCC_POS *tmp, MCC_GETS *src ) {
	printf( " Length: %ld", src->text.vec.use );
	putchar('\n');
	mcc_vecsize( &(src->text.vec), 0, sizeof(mcc_ch8_t) );
#if COPY_TEST_TEXT
	mcc_vecsize( &(tmp->vec), 0, csize );
#endif
	return ret;
}

int _testc(
	void *text, size_t size, size_t csize,
	func_mcc_gets _gets ) {
	int ret;
	MCC_POS tmp = {0};
	MCC_GETS src = setup_test( &tmp, text, size, csize, "Char/s", _gets );
	MCC_GETC chr = {0};
	chr.src = &src;
	while ( (ret = mcc_getc( &chr )) == EXIT_SUCCESS )
		printf( " '%c'", chr.c[0] );
	return clean_test( ret, csize, &tmp, &src );
}
int _test(
	void *text, size_t size, size_t csize,
	char *label, func_mcc_gets _gets ) {
	int ret;// = 0~62~z;
	MCC_POS tmp = {0};
	MCC_GETS src = setup_test( &tmp, text, size, csize, label, _gets );
	while ( (ret = mcc_gets(&src)) == EXIT_SUCCESS )
		(void)printf( " '%s'", (char*)(src.text.vec.mem.addr) );
	return clean_test( ret, csize, &tmp, &src );
}
int test_ch8str( mcc_ch8_t *text, char *label, func_mcc_gets _gets ) {
	size_t csize = sizeof(mcc_ch8_t);
	return _test(
		text, csize * (mcc_ch8len(text)+1), csize, label, _gets );
}
int test_wcsstr( wchar_t *text, char *label, func_mcc_gets _gets ) {
	size_t csize = sizeof(wchar_t);
	return _test(
		text, csize * (mcc_wcslen(text)+1), csize, label, _gets );
}
mcc_ch8_t *testing8 = u8"Testing, testing, 123";
wchar_t *wtesting = L"Testing, testing, 123";
char *test_label[] = { "Text", "Line/s", "Word/s", NULL };
func_mcc_gets test_gets8[] = {
	(func_mcc_gets)mcc_ch8gettext,
	(func_mcc_gets)mcc_ch8getline,
	(func_mcc_gets)mcc_ch8getword,
NULL };
func_mcc_gets wtest_gets[] = {
	(func_mcc_gets)mcc_wcsgettext,
	(func_mcc_gets)mcc_wcsgetline,
	(func_mcc_gets)mcc_wcsgetword,
NULL };
int main() {
	int ret = open_mcc_encodings(), i;
	size_t count, size8, wsize;
	if ( ret != EXIT_SUCCESS ) {
		FAIL( stderr, ret, "" );
		return EXIT_FAILURE;
	}
	count = mcc_wcslen( wtesting ) + 1;
	size8 = count * sizeof(mcc_ch8_t);
	wsize = count * sizeof(wchar_t);
	for ( i = 0; test_label[i]; ++i ) {
		ret = _test(
			testing8, size8, sizeof(mcc_ch8_t),
			test_label[i], test_gets8[i] );
		if ( ret != EXIT_SUCCESS && ret != ENODATA ) {
			shut_mcc_encodings();
			FAIL( stderr, ret, "" );
			return EXIT_FAILURE;
		}
		ret = _test(
			wtesting, wsize, sizeof(wchar_t),
			test_label[i], wtest_gets[i] );
		if ( ret != EXIT_SUCCESS && ret != ENODATA ) {
			shut_mcc_encodings();
			FAIL( stderr, ret, "" );
			return EXIT_FAILURE;
		}
	}
	ret = _testc( testing8, size8, sizeof(mcc_ch8_t), test_gets8[0] );
	ret = _testc( wtesting, wsize, sizeof(wchar_t), wtest_gets[0] );
	shut_mcc_encodings();
	return EXIT_SUCCESS;
}
