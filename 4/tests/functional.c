
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

	PT_TEST(test_heap_function) {

#if defined(__APPLE__)
		var (^empty_function)(var) = ^ var (var args) { return None; };
#else
		var empty_function(var args) { return None; }
#endif

		var f = new(Function, $(Function, empty_function));
		PT_ASSERT(f);
		delete(f);
	}

	PT_TEST(test_function_assign) {

#if defined(__APPLE__)
		var (^empty_function)(var) = ^ var (var args) { return None; };
		var (^empty_function2)(var) = ^ var (var args) { return None; };
#else
		var empty_function(var args) { return None; };
		var empty_function2(var args) { return None; };
#endif

		var f1 = new(Function, $(Function, empty_function));
		var f2 = new(Function, $(Function, empty_function2));

		PT_ASSERT(((FunctionData *)f1)->func isnt ((FunctionData *)f2)->func);

		assign(f1, f2);

		PT_ASSERT(((FunctionData *)f1)->func == ((FunctionData *)f2)->func);

		delete(f1);
		delete(f2);
	}

	PT_TEST(test_function_copy) {

#if defined(__APPLE__)
		var (^empty_function)(var) = ^ var (var args) { return None; };
#else
		var empty_function(var args) { return None; };
#endif

		var f1 = new(Function, $(Function, empty_function));
		var f2 = copy(f1);

		PT_ASSERT(((FunctionData *)f1)->func is ((FunctionData *)f2)->func);

		delete(f1);
		delete(f2);
	}

	PT_TEST(test_call) {

#if defined(__APPLE__)
		var (^empty_function)(var) = ^ var (var args) { return None; };
		var (^empty_function2)(var) = ^ var (var args) { return None; };
#else
		var empty_function(var args) { return None; };
		var empty_function2(var args) { return None; };
#endif

		var result1 = call($(Function, empty_function));
		var result2 = call($(Function, empty_function2));

		PT_ASSERT(result1 is None);
		PT_ASSERT(result2 is None);
	}

	PT_TEST(test_call_with) {

#if defined(__APPLE__)
		var (^asserts_args)(var) = ^ var (var args) {
			PT_ASSERT(at(args, 0));
			PT_ASSERT(at(args, 1));
			return None;
		};
#else
		var asserts_args(var args) {
			PT_ASSERT(at(args, 0));
			PT_ASSERT(at(args, 1));
			return None;
		}
#endif

		var args = new(List, $(Int, 1), $(Int, 5));
		var assert_func = $(Function, asserts_args);
		var result = call_with(assert_func, args);
		PT_ASSERT(result is None);
		delete(args);
	}

	PT_TEST(test_call_vl) {

#if defined(__APPLE__)
		var (^asserts_args)(var) = ^ var (var args) {
			PT_ASSERT(at(args, 0));
			PT_ASSERT(at(args, 1));
			return None;
		};
#else
		var asserts_args(var args) {
			PT_ASSERT(at(args, 0));
			PT_ASSERT(at(args, 1));
			return None;
		}
#endif

		var_list args = var_list_new($(Int, 1), $(Int, 5));
		var assert_func = $(Function, asserts_args);
		var result = call_vl(assert_func, args);
		PT_ASSERT(result is None);
	}

	PT_TEST(test_lambda) {
		var out = new(String, $(String, ""));

		lambda(hello_name, args) {
			var name = cast(at(args, 0), String);
			var out = cast(at(args, 1), String);
			assign(out, name);
			return None;
		};

		call(hello_name, $(String, "Hello Bob!"), out);
		PT_ASSERT( eq(out, $(String, "Hello Bob!")) );
		delete(out);
	}

	PT_TEST(test_lambda_id) {
		lambda(return_new_int1, args) {
			return new(Int, $(Int, 1));
		};

		lambda_id(return_new_int2, return_new_int1);

		var res1 = call(return_new_int1, None);
		var res2 = call(return_new_int2, None);

		PT_ASSERT(as_long(res1) is 1);
		PT_ASSERT(as_long(res2) is 1);
		PT_ASSERT(as_long(res1) == as_long(res2));

		delete(res1);
		delete(res2);
	}

	PT_TEST(test_lambda_const) {
		lambda_const(return_some, Some);
		var res = call(return_some, None);
		PT_ASSERT(res is Some);
	}

	PT_TEST(test_lambda_compose) {
		lambda(return_some, args) {
			return new(List, Some);
		};

		lambda (return_arg0, args) {
			var fst = at(args, 0);
			delete(args);
			return fst;
		}

		lambda_compose(return_compose, return_arg0, return_some);
		var res = call(return_compose, None);
		PT_ASSERT(res is Some);
	}

	PT_TEST(test_lambda_flip) {
		lambda(return_first, args) {
			return at(args, 0);
		};

		lambda_flip(return_first_flip, return_first);

		var arg1 = $(Int, 5);
		var arg2 = $(Int, 10);

		var res1 = call(return_first, arg1, arg2);
		var res2 = call(return_first_flip, arg1, arg2);

		PT_ASSERT(res1 is arg1);
		PT_ASSERT(res2 is arg2);
	}
}

