#ifndef INC_MCC_GET_H
#define INC_MCC_GET_H
#include "mcc_bitsof.h"
#include "mcc_err.h"
#include "mcc_mem.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <ctype.h>
#include <string.h>
#include <wchar.h>
#include <uchar.h>
#include <locale.h>
#endif
#include <iconv.h>
#include <stdlib.h>

#define _2STR( DATA ) #DATA
#define TOSTR( DATA ) _2STR( DATA )
#define _2WCS( DATA ) L#DATA
#define TOWCS( DATA ) _2WCS( DATA )
#define _2CH88( DATA ) u8#DATA
#define TOCH88( DATA ) _2CH88( DATA )
#define _2CH816( DATA ) u#DATA
#define TOCH816( DATA ) _2CH816( DATA )
#define _2CH832( DATA ) U#DATA
#define TOCH832( DATA ) _2CH832( DATA )

#if CHAR_BIT >= 8
typedef char mcc_ch8_t;
#define MCC_CH8_MAX CHAR_MAX
#define MCC_CH8_MIN CHAR_MIN
#else
typedef uint_least8_t mcc_ch8_t;
#define MCC_CH8_MAX UINT_LEAST8_MAX
#define MCC_CH8_MIN 0
#endif

#ifdef __char16_t_defined
typedef char16_t mcc_c16_t;
#else
typedef uint_least16_t mcc_c16_t;
#endif

#ifdef __char32_t_defined
typedef char32_t mcc_c32_t;
#else
typedef uint_least32_t mcc_c32_t;
#endif

#define MCC_C16_MAX UINT_LEAST16_MAX
#define MCC_C32_MAX UINT_LEAST32_MAX

typedef enum mcc_encoding {
	mcc_encoding_str = 0,
	mcc_encoding_ch8,
	mcc_encoding_c16,
	mcc_encoding_c32,
	mcc_encoding_wcs,
	mcc_encoding_count
} mcc_encoding_t;
/**
	@brief '8','u' & 'U' for CH8-8, CH8-16 & CH8-32 respectively.
	Unsupported values will default to mcc_encoding_std variants
**/
extern int mcc_encoding_type;
extern int mcc_encoding_node;
extern char const *env_LANG;
extern char const *mcc_encoding_names[ mcc_encoding_count ];
#ifndef invalid_iconv
#define invalid_iconv ((iconv_t)-1)
#endif
#ifndef skip_iconv
#define skip_iconv ((iconv_t)-2)
#endif
int find_mcc_encoding();
void shut_mcc_encodings();
int open_mcc_encodings();

typedef MCC_POS MCC_STR, MCC_WCS, MCC_CH8, MCC_C16, MCC_C32;

enum {
	MCC_CHAR_TYPE_ASCII = 0,
	MCC_CHAR_TYPE_OTHER,
	MCC_CHAR_TYPE_11BIT,
	MCC_CHAR_TYPE_16BIT,
	MCC_CHAR_TYPE_21BIT,
	MCC_CHAR_TYPE_15BIT,
	MCC_CHAR_TYPE_24BIT,
	MCC_CHAR_TYPE_SPACE,
	MCC_CHAR_TYPE_PUNCT,
	MCC_CHAR_TYPE_DIGIT,
	MCC_CHAR_TYPE_UPPER,
	MCC_CHAR_TYPE_LOWER,
	MCC_CHAR_TYPE_COUNT
};

#if defined BUFSIZ && BUFSIZ < 512
#define MCC_BUFSIZ BUFSIZ
#else
#define MCC_BUFSIZ 512
#endif

#define MCC_CH8_LENG 7
#define MCC_CH8_UPTO (MCC_CH8_LENG + 1)
#define MCC_CH8_SIZE (MCC_CH8_UPTO * sizeof(mcc_ch8_t))
typedef mcc_ch8_t mcc_utf_t[MCC_CH8_UPTO];
typedef long (*func_mcc_cleng)( void const * c );
typedef int (*func_mcc_ctype)( void const * c );
typedef long (*func_mcc_len)( void const * txt );
typedef long (*func_mcc_tell)( void * src );
typedef int (*func_mcc_seek)( void * src, long pos, int whence );
typedef int (*func_mcc_getc)( void * src, mcc_utf_t dst );
typedef int (*func_mcc_gets)( void * src, MCC_CH8 *dst );

typedef struct mcc_char_info {
	size_t size;
	char const * name;
	iconv_t ch8dst, ch8src;
	func_mcc_cleng cleng;
	func_mcc_ctype ctype;
	mcc_encoding_t enc;
} mcc_char_info_t;

int mcc_char_info_test( mcc_char_info_t mcc_char_info );

extern mcc_char_info_t mcc_common_char_info[mcc_encoding_count];

typedef struct mcc_iconv_mem {
	size_t left, done, size;
	char *addr;
} MCC_ICONV_MEM;
typedef struct mcc_iconv_tok {
	iconv_t cd;
	MCC_ICONV_MEM src, dst;
} MCC_ICONV_TOK;

MCC_ICONV_TOK mcc_iconv_new_tok( iconv_t cd );
MCC_ICONV_MEM mcc_iconv_tok_mem(
	MCC_ICONV_MEM prv, void *src, size_t size );
int mcc_iconv( MCC_ICONV_TOK *tok );

typedef struct _mcc_gets {
	MCC_CH8 text;
	void *src;
	func_mcc_tell tell;
	func_mcc_seek seek;
	func_mcc_gets gets;
} MCC_GETS;

int mcc_gets_init( MCC_GETS *dst );
int mcc_gets_test( MCC_GETS *src );
int mcc_gets( MCC_GETS *src );
int mcc_getc( MCC_GETS *src, mcc_utf_t c, long *len );

long mcc_strcleng( char const *c );
long mcc_wcscleng( wchar_t const *c );
long mcc_ch8cleng( mcc_ch8_t const *c );
long mcc_c16cleng( mcc_c16_t const *c );
long mcc_c32cleng( mcc_c32_t const *c );
long mcc_strlen( char const *txt );
long mcc_wcslen( wchar_t const *txt );
long mcc_ch8len( mcc_ch8_t const *txt );
long mcc_c16len( mcc_c16_t const *txt );
long mcc_c32len( mcc_c32_t const *txt );
int mcc_strctype( char c );
int mcc_wcsctype( wchar_t c );
int mcc_ch8ctype( mcc_ch8_t c );
int mcc_c16ctype( char16_t c );
int mcc_c32ctype( char32_t c );

long mcc_ch8cpy(
	mcc_ch8_t *dst, long cap, mcc_ch8_t const *src, long len );

#define mcc_strtell mcc_postell
#define mcc_wcstell mcc_postell
#define mcc_ch8tell mcc_postell
#define mcc_c16tell mcc_postell
#define mcc_c32tell mcc_postell
#define mcc_strseek mcc_posseek
#define mcc_wcsseek mcc_posseek
#define mcc_ch8seek mcc_posseek
#define mcc_c16seek mcc_posseek
#define mcc_c32seek mcc_posseek

int mcc_strgettext( MCC_STR *src, MCC_CH8 *dst );
int mcc_wcsgettext( MCC_WCS *src, MCC_CH8 *dst );
int mcc_ch8gettext( MCC_CH8 *src, MCC_CH8 *dst );
int mcc_c16gettext( MCC_C16 *src, MCC_CH8 *dst );
int mcc_c32gettext( MCC_C32 *src, MCC_CH8 *dst );
int mcc_strgetline( MCC_STR *src, MCC_CH8 *dst );
int mcc_wcsgetline( MCC_WCS *src, MCC_CH8 *dst );
int mcc_ch8getline( MCC_CH8 *src, MCC_CH8 *dst );
int mcc_c16getline( MCC_C16 *src, MCC_CH8 *dst );
int mcc_c32getline( MCC_C32 *src, MCC_CH8 *dst );
int mcc_strgetword( MCC_STR *src, MCC_CH8 *dst );
int mcc_wcsgetword( MCC_WCS *src, MCC_CH8 *dst );
int mcc_ch8getword( MCC_CH8 *src, MCC_CH8 *dst );
int mcc_c16getword( MCC_C16 *src, MCC_CH8 *dst );
int mcc_c32getword( MCC_C32 *src, MCC_CH8 *dst );
int mcc_strgetchar( MCC_STR *src, mcc_utf_t dst, long *len );
int mcc_wcsgetchar( MCC_WCS *src, mcc_utf_t dst, long *len );
int mcc_ch8getchar( MCC_CH8 *src, mcc_utf_t dst, long *len );
int mcc_c16getchar( MCC_C16 *src, mcc_utf_t dst, long *len );
int mcc_c32getchar( MCC_C32 *src, mcc_utf_t dst, long *len );
int mcc_fstrgettext( FILE *src, MCC_CH8 *dst );
int mcc_fwcsgettext( FILE *src, MCC_CH8 *dst );
int mcc_fch8gettext( FILE *src, MCC_CH8 *dst );
int mcc_fc16gettext( FILE *src, MCC_CH8 *dst );
int mcc_fc32gettext( FILE *src, MCC_CH8 *dst );
int mcc_fstrgetline( FILE *src, MCC_CH8 *dst );
int mcc_fwcsgetline( FILE *src, MCC_CH8 *dst );
int mcc_fch8getline( FILE *src, MCC_CH8 *dst );
int mcc_fc16getline( FILE *src, MCC_CH8 *dst );
int mcc_fc32getline( FILE *src, MCC_CH8 *dst );
int mcc_fstrgetword( FILE *src, MCC_CH8 *dst );
int mcc_fwcsgetword( FILE *src, MCC_CH8 *dst );
int mcc_fch8getword( FILE *src, MCC_CH8 *dst );
int mcc_fc16getword( FILE *src, MCC_CH8 *dst );
int mcc_fc32getword( FILE *src, MCC_CH8 *dst );
int mcc_fstrgetchar( FILE *src, mcc_utf_t dst, long *len );
int mcc_fwcsgetchar( FILE *src, mcc_utf_t dst, long *len );
int mcc_fch8getchar( FILE *src, mcc_utf_t dst, long *len );
int mcc_fc16getchar( FILE *src, mcc_utf_t dst, long *len );
int mcc_fc32getchar( FILE *src, mcc_utf_t dst, long *len );
int mcc_ch8cmp( mcc_ch8_t const *a, mcc_ch8_t const *b, long cap );
int mcc_ich8cmp( mcc_ch8_t const *a, mcc_ch8_t const *b, long cap );
int mcc_gets_validate( MCC_GETS *src );
int mcc_gets( MCC_GETS *src );

typedef struct mcc_num {
	char pfx[4]; /* For if statements */
	size_t base;
	char sfx[4];
	ullong num;
	ullong dec; /* After dot */
	ullong exp; /* After E/P */
	union {
		ullong btw;
		long double fpn;
	}
} MCC_NUM;

/** @brief reads a series of characters as a number
 * @param base Less than 2 causes mcc_getnum() to read the text as a
 * number literal (e.g. 0xFFF or 0.1e+10), anything else and it will
 * read it as JUST a number, no +/- or .
 * @param lowislow Only paid attention to when base is greater than 36,
 * tells mcc_getnum() to read A-Z as 36+ and a-z as 10 to 35, otherwise
 * the reverse is done if false, A-Z will read as 10-35 like normal and
 * a-z will read as 36+
 * @param min_dig Chucks an error if read characters is lower than this
 * @param max_dig Stops reading when read characters reaches this hieght
**/
int mcc_getnum(
	MCC_GETS *src, MCC_NUM *dst,
	mcc_utf_t C, size_t base,
	bool lowislow, long min_dig, long max_dig
);

// https://www.geeksforgeeks.org/generating-random-number-range-c/
#define mcc_rand( min, max ) ((rand() % ((max) - (min) + 1)) + (min))

#endif /* INC_NEXT_H */
