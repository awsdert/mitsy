#include <mcc/core/tsc.h>
#include <mcc/core/get.h>
#include <mcc/core/base62.h>
#include "test/core/vint.h"
#include "test/core/vflt.h"
int main() {
	int ret = EXIT_SUCCESS;
	puts("Starting unit tests...\n");
	ret = test_vint();
	if ( ret != EXIT_SUCCESS ) return EXIT_FAILURE;
	ret = test_vflt();
	return ( ret == EXIT_SUCCESS ) ? EXIT_SUCCESS : EXIT_FAILURE;
}
