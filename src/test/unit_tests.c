#include <mcc/core/tsc.h>
#include <mcc/core/get.h>
#include <mcc/core/base62.h>
#include "core/vint.c"
int main() {
	int ret = test_vint();
	return ( ret == EXIT_SUCCESS ) ? EXIT_SUCCESS : EXIT_FAILURE;
}
