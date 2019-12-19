#ifndef INC_MCC_MALLOC_H
#define INC_MCC_MALLOC_H

/* TODO: create wrappers for
 * return (HeapAlloc(
 * 	HeapCreate(..., size + sizeof(HANDLE) )) + sizeof(HANDLE*))
 * HeapRealloc( (HANDLE**)(ptr - sizeof(HANDLE)), size )
 * h = (HANDLE*)(ptr - sizeof(HANDLE));
 * HeapFree( ... ) 
 * HeapDestroy( h ) */

#ifdef MCC_SYS_WINDOWS_OS
#include <windows.h>
void* malloc(size_t __size);
void* realloc(void *__ptr, size_t __size);
void free(void *__ptr);
#elif defined( MCC_SYS_LINUX )
#include <linux/malloc.h>
#else
#include <malloc.h>
#endif

#endif
