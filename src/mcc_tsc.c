#include "mcc_tsc.h"

mcc_tsc_t mcc_tsc() {
#if defined( MCC_CPU_x86 )
	ullong a, d;
	__asm__ __volatile__ (
#if (defined(MCC_CPU_x86_64) || defined(MCC_CPU_SSE2)) & defined( SYNC_MEM )
	"mfence\n\t"
#endif
	"xorl %%eax,%%eax\n\t"
	"cpuid\n\t"
	"rdtsc" : "=a" (a), "=d" (d) ::
#ifdef MCC_CPU_x86_64
		"%rbx", "%rcx"
#else
		"%ebx", "%ecx"
#endif
	);
	return a | (d << 32);
#else
	return time(NULL);
#endif
}
