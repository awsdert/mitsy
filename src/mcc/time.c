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
long mcc__rnd( mcc_rnd_t *ctx, long min, long max ) {
	mcc_rnd_t bit = 1, seed = time(NULL), mov = 0;
	long val;
	if ( ctx ) mov = *ctx;
	bit <<= mov++;
	if ( !bit || bit > seed ) {
		bit = 1;
		mov = 0;
	}
	val = (seed & bit) ? 1 : 0;
	bit <<= 1;
	if ( ctx ) *ctx = mov;
	if ( min > max ) min = max;
	if ( min != max ) {
		seed *= clock();
		bit = ~((~0u) << mov);
		val = (seed & bit);
		if ( val > max ) val = max;
		val -= (min >= 0) ? min : -min;
		return ( val < min ) ? min : val;
	}
	return val ? max : min;
}
