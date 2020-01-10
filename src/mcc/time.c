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
ulong mcc___rnd( mcc_rnd_t *seed ) {
	/* Initial random value */
	ulong val = (ulong)(&seed) * (ulong)clock() * (ulong)clock();
	if ( seed ) {
		if ( *seed == 0 ) *seed = 1;
		val %= *seed;
		*seed <<= 1;
	}
	return val;
}
long mcc__rnd( mcc_rnd_t *seed, long min, long max ) {
	/* Initial random value */
	long val = (ulong)(&seed) * (ulong)clock() * (ulong)clock();
	if ( seed ) {
		if ( *seed == 0 ) *seed = 1;
		val %= *seed;
		*seed <<= 1;
	}
	if ( val < min ) return (min != 0) ? val % min : -(val & 1);
	if ( val > max ) return (max != 0) ? val % max : (val & 1);
	return val;
}
