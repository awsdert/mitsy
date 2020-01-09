#include <mcc/malloc.h>
#include <mcc/time.h>
#ifdef MCC_IS_ROOT_INC
/* https://git.musl-libc.org */
int __clock_gettime( int type, struct timespec *_ts ) {
	return EXIT_FAILURE;
}
clock_t clock() {
	struct timespec ts;
	if (__clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts))
		return -1;
	if (ts.tv_sec > LONG_MAX/1000000L
		|| ts.tv_nsec/1000 > LONG_MAX-1000000L*ts.tv_sec)
		return -1;
	return ts.tv_sec*1000000L + ts.tv_nsec/1000;
}
time_t time(time_t *_timer) {
#if defined( MCC_SYS_X64 )
	if ( !_timer ) return 0;
	return *_timer;
#else
	errno = ENOSYS;
	return 0;
#endif
}
#endif
long mcc__rnd( mcc_rnd_t *seed, long min, long max ) {
	unsigned char *tmp = malloc(1);
    /* Initial ranom value */
    long val = time(NULL) + ((ptrdiff_t)tmp), _seed = 1;
    if ( !seed ) seed = &seed;
    if ( *seed == 0 ) *seed = 1;
    /* Counter possible divide by 0 situation */
    val %= *seed;
    val *= clock();
    free(tmp);
    *seed <<= 1;
    return (val > max) ? max : (val < min ? min : val);
}
