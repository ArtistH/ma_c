
#include "ptest.h"
#include "Cello.h"

PT_SUITE(suite_functional) {

	PT_TEST(test_stack_function) {

#if defined(__APPLE__)
		var (^empty_function)(var) = ^ var (var args) { return None; };
#else
		var empty_function(var args) { return None; }
#endif

		var f = $(Function, empty_function);
		PT_ASSERT(f);
	}
}
