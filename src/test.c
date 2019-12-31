#include <mcc/core/tsc.h>
#include <mcc/core/get.h>
#include <mcc/core/base62.h>
#include "test/core/vint.h"
int main() {
	int ret = EXIT_SUCCESS;
	puts("Starting unit tests...\n");
	ret = test_vint();
	return ( ret == EXIT_SUCCESS ) ? EXIT_SUCCESS : EXIT_FAILURE;
}
