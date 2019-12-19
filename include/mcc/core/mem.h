#ifndef INC_MCC_MEM_H
#define INC_MCC_MEM_H
#include <mcc/core/err.h>
#include <mcc/bitsof.h>
#include <mcc/malloc.h>

int mcc_set_int_max( void *dst, size_t bits );
int mcc_set_int_min( void *dst, size_t bits );
int mcc_set_uint_max( void *dst, size_t bits );

typedef struct mcc_mem {
	size_t size;
	void 	*addr;
} MCC_MEM;

typedef struct mcc_vec {
	long cap;
	long use;
	MCC_MEM mem;
} MCC_VEC;

typedef struct mcc_pos {
	long pos;
	MCC_VEC vec;
} MCC_POS;

int mcc_rawfill( void *dst, char c, size_t size );
int mcc_rawcomp(
	void const *a, size_t a_size,
	void const *b, size_t b_size );
size_t mcc_rawcopy(
	void *dst, size_t dst_size,
	void const *src, size_t src_size );
MCC_MEM mcc_tomem( void *addr, size_t size );
int mcc_memsize( MCC_MEM *mcc_mem, size_t size );
MCC_MEM mcc_tovec( void *addr, size_t size, size_t pern );
int mcc_vecsize( MCC_VEC *mcc_vec, size_t size, size_t pern );
int mcc_vecclamp( MCC_VEC *mcc_vec );
void* mcc_vecnode( MCC_VEC *mcc_vec, long pos, size_t pern );
int mcc_posclamp( MCC_POS *mcc_pos );
void* mcc_posnode( MCC_POS *mcc_pos, size_t pern );
long mcc_postell( MCC_POS *mcc_pos );
int mcc_posseek( MCC_POS *mcc_pos, long pos, int whence );
typedef int (*func_mcc__last)( void *src );
typedef size_t (*func_mcc__read)(
	void *dst, size_t pern, size_t upto, void *src );
/** @brief Replicates feof() behaviour **/
int mcc_poslast( MCC_POS *mcc_pos );
/** @brief Replicates fread() behaviour
 * @return
 * if dst is NULL errno is set and 0 returned.
 * if src is or its memory is NULL or its size is 0 then 0 is returned.
 * bytes to read (node size * upto) will be clamped to the difference
 * between the current pos and the final position (src->vec.use)
**/
size_t mcc_posread( void *dst, size_t pern, size_t upto, MCC_POS *src );

#endif
