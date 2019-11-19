#ifndef INC_BITSOF_H
#define INC_BITSOF_H
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>

#ifndef __bitsof_defined
#define __bitsof_defined
#define bitsof(T) (sizeof(T) * CHAR_BIT)
#endif

#ifndef __schar_defined
#define __schar_defined
typedef signed char schar;
#endif
#ifndef __uchar_defined
#define __uchar_defined
typedef unsigned char uchar;
#endif
#ifndef __sllong_defined
#define __sllong_defined
typedef signed long long sllong;
#endif
#ifndef __ullong_defined
#define __ullong_defined
typedef unsigned long long ullong;
#endif

#endif /* INC_BITSOF_H */
