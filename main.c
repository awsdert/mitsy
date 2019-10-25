#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#define _2STR( DATA ) #DATA
#define TOSTR( DATA ) _2STR( DATA )
#define OUT_STR TOSTR( OUT )

int call0() { return 0; }
int call1() { return 1; }
typedef int (*call_t)();

typedef volatile uint_least64_t counter_t;
#if defined( __x86_64__ ) || defined( __i386__ )
counter_t INIT_TSC( void ) {
	uint32_t a, d;
	__asm__ __volatile__ (
#if (defined(__x86_64__) || defined( __SSE2__ )) & defined( SYNC_MEM )
	"mfence\n\t"
#endif
	"xorl %%eax,%%eax\n\t"
	"cpuid\n\t"
	"rdtsc" : "=a" (a), "=d" (d) ::
#ifdef __x86_64__
		"%rbx", "%rcx"
#else
		"%ebx", "%ecx"
#endif
	);
	return a | ((uint64_t)d << 32);
}
void KILL_TSC( counter_t *cptr )
{
	uint32_t a, d;
	__asm__ __volatile__ (
		"cpuid\n\t"
		"rdtscp" : "=a" (a), "=d" (d) ::
#ifdef _x86_64__
		"%rcx"
#else
		"%ecx"
#endif
	);
	*cptr = (a | ((uint64_t)d << 32)) - *cptr;;
}
#else
#define INIT_TSC() time(NULL)
#define KILL_TSC(P) (*(P) = time(NULL))
#endif

typedef struct main_ctx {
	time_t timed;
	int a;
	char *arg0;
	_Bool name_included;
	const char *name;
	counter_t init;
	counter_t stop;
	counter_t tick;
} main_ctx_t;

void print_timed( main_ctx_t *ctx ) {
	KILL_TSC(&ctx->stop);
	ctx->tick = clock();
#define VAL(N) N " = %020" PRIu64
	(void)printf("Timed Action for '%s':"
		VAL("init") VAL(", stop") VAL(", diff") VAL(", tick") "\n",
		ctx->name, ctx->init, ctx->stop, ctx->stop - ctx->init, ctx->tick
	);
#undef VAL
}
int try_cond( main_ctx_t *ctx ) {
	ctx->stop = 0;
	ctx->tick = 0;
	ctx->init = INIT_TSC();
	ctx->a = ctx->name_included ? 1 : 0;
	ctx->name = "?:";
	print_timed(ctx);
	return ctx->a;
}

int try_query( main_ctx_t *ctx ) {
	ctx->stop = 0;
	ctx->tick = 0;
	ctx->init = INIT_TSC();
	if ( ctx->name_included ) ctx->a = 1;
	ctx->name = "if";
	print_timed(ctx);
	return ctx->a;
}
int try_switch( main_ctx_t *ctx ) {
	ctx->stop = 0;
	ctx->tick = 0;
	ctx->init = INIT_TSC();
	switch ( ctx->name_included ) {
	case 1: ctx->a = 1; break;
	}
	ctx->name = "{}";
	print_timed(ctx);
	return ctx->a;
}
int try_array( main_ctx_t *ctx ) {
	int array[2] = { 0, 1 };
	ctx->stop = 0;
	ctx->tick = 0;
	ctx->init = INIT_TSC();
	ctx->a = array[ctx->name_included];
	ctx->name = "[]";
	print_timed(ctx);
	return ctx->a;
}
int try_calls( main_ctx_t *ctx ) {
	call_t calls[2] = { call0, call1 };
	ctx->stop = 0;
	ctx->tick = 0;
	ctx->init = INIT_TSC();
	ctx->a = calls[ctx->name_included]();
	ctx->name = "()";
	print_timed(ctx);
	return ctx->a;
}
typedef int (*try_test_t)( main_ctx_t *ctx );

// https://www.geeksforgeeks.org/generating-random-number-range-c/
#define BETWEEN( MIN, MAX ) ((rand() % ((MAX) - (MIN) + 1)) + (MIN))

int main( int argc, char *argv[]  ) {
		main_ctx_t ctx = {0};
    int a = 0, j;
    try_test_t tests[5] =
			{ try_cond, try_query, try_switch, try_array, try_calls };
		ctx.timed = time(&ctx.timed);
    ctx.name_included =  !!(strstr( argv[0], OUT_STR ));
    try_cond( &ctx );
    try_query( &ctx );
    try_switch( &ctx );
    try_array( &ctx );
    try_calls( &ctx );
    for ( a = 0; a < 10; ++a ) {
			j = BETWEEN(0,4);
			tests[j]( &ctx );
		}
    return 0;
}
