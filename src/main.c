#include "mcc_tsc.h"
#include "mcc_get.h"

#define OUT_STR TOSTR( OUT )

int call0() { return 0; }
int call1() { return 1; }
typedef int (*call_t)();

typedef struct main_ctx {
	time_t timed;
	int a;
	char *arg0;
	_Bool name_included;
	const char *name;
	mcc_tsc_t init;
	mcc_tsc_t stop;
	mcc_tsc_t tick;
} main_ctx_t;

void print_timed( main_ctx_t *ctx ) {
	ctx->stop = mcc_tsc();
	ctx->tick = mcc_tsc();
#define VAL(N) N " = %020llu"
	(void)printf("Timed Action for '%s':"
		VAL("init") VAL(", stop") VAL(", diff") VAL(", tick") "\n",
		ctx->name, ctx->init, ctx->stop, ctx->stop - ctx->init, ctx->tick
	);
#undef VAL
}
int try_cond( main_ctx_t *ctx ) {
	ctx->stop = 0;
	ctx->tick = 0;
	ctx->init = mcc_tsc();
	ctx->a = ctx->name_included ? 1 : 0;
	ctx->name = "?:";
	print_timed(ctx);
	return ctx->a;
}

int try_query( main_ctx_t *ctx ) {
	ctx->stop = 0;
	ctx->tick = 0;
	ctx->init = mcc_tsc();
	if ( ctx->name_included ) ctx->a = 1;
	ctx->name = "if";
	print_timed(ctx);
	return ctx->a;
}
int try_switch( main_ctx_t *ctx ) {
	ctx->stop = 0;
	ctx->tick = 0;
	ctx->init = mcc_tsc();
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
	ctx->init = mcc_tsc();
	ctx->a = array[ctx->name_included];
	ctx->name = "[]";
	print_timed(ctx);
	return ctx->a;
}
int try_calls( main_ctx_t *ctx ) {
	call_t calls[2] = { call0, call1 };
	ctx->stop = 0;
	ctx->tick = 0;
	ctx->init = mcc_tsc();
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
			j = mcc_rand(0,4);
			tests[j]( &ctx );
		}
    return 0;
}
