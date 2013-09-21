
#include "ptest.h"
#include "Cello.h"

local var TestType;

data {
	var type;
	int test_data;
} TestTypeData;

local var TestType_New(var self, var_list vl) {
	TestTypeData* ttd = cast(self, TestType);
	ttd->test_data = as_long(var_list_get(vl));
	return self;
}

local var TestType_Delete(var self) {
	return self;
}

local size_t TestType_Size(void) {
	return sizeof(TestTypeData);
}

local var TestType_Eq(var self, var obj) {
	TestTypeData* lhs = cast(self, TestType);
	TestTypeData* rhs = cast(obj, TestType);
	if (lhs->test_data == rhs->test_data) {
		return True;
	} else {
		return False;
	}
}

instance(TestType, New) = { TestType_New, TestType_Delete, TestType_Size };
instance(TestType, Eq) = { TestType_Eq };

class {
	var (*return_true)(var);
} ReturnTrue;

local var return_true(var self) {
	return type_class_method(self, ReturnTrue, return_true, self);
}

global var IntParent;

local var IntParent_ReturnTrue(var self) {
	return True;
}

instance(IntParent, ReturnTrue) = { IntParent_ReturnTrue };

var IntParent = type_data {
	type_begin(IntParent),
	type_entry(IntParent, ReturnTrue),
	type_end(IntParent),
};

PT_SUITE(suite_core) {

	PT_TEST(test_type) {
		PT_ASSERT(type_of($(Int, 1)) is Int);
		PT_ASSERT(type_of($(Real, 1.0)) is Real);
		PT_ASSERT(type_of(True) is Bool);
		PT_ASSERT(type_of(False) is Bool);
		PT_ASSERT(type_of(Int) is Type);
		PT_ASSERT(type_of(Real) is Type);
		PT_ASSERT(type_of(Type) is Type);
	}

	PT_TEST(test_cast) {
		var x = $(Int, 1);
		var y = $(Real, 2.0);

		x = cast(x, Int);
		y = cast(y, Real);

		PT_ASSERT(x);
		PT_ASSERT(y);
	}

	PT_TEST(test_new) {
		var x = new(Int, $(Int, 1));

		PT_ASSERT(x);
		PT_ASSERT(type_of(x) is Int);
		PT_ASSERT(as_long(x) is 1);

		delete(x);

		var y = $(Real, 0.0);
		construct(y, $(Real, 1.0));
		PT_ASSERT(as_double(y) is 1.0);

		var z = allocate(String);
		PT_ASSERT(z);
		construct(z, $(String, "Hello"));
		PT_ASSERT_STR_EQ(as_str(z), "Hello");
		z = destruct(z);
		deallocate(z);
	}

	PT_TEST(test_assign) {

		/* Integers */

		var x = new(Int, $(Int, 10));
		var y = new(Int, $(Int, 20));

		PT_ASSERT(as_long(x) is 10);
		PT_ASSERT(as_long(y) is 20);
		PT_ASSERT(x isnt y);

		assign(x, y);

		PT_ASSERT(as_long(x) is 20);
		PT_ASSERT(as_long(y) is 20);
		PT_ASSERT(x isnt y);

		delete(x);
		delete(y);

		/* Strings */

		var xs = new(String, $(String, "Hello"));
		var ys = new(String, $(String, "There"));

		PT_ASSERT_STR_EQ(as_str(xs), "Hello");
		PT_ASSERT_STR_EQ(as_str(ys), "There");
		PT_ASSERT(as_str(xs) isnt as_str(ys));
		PT_ASSERT(xs isnt ys);

		assign(xs, ys);

		PT_ASSERT_STR_EQ(as_str(xs), "There");
		PT_ASSERT_STR_EQ(as_str(ys), "There");
		PT_ASSERT(as_str(xs) isnt as_str(ys));
		PT_ASSERT(xs isnt ys);

		delete(xs);
		delete(ys);
	}

	PT_TEST(test_copy) {
		var x = new(String, $(String, "Hello"));
		var y = copy(x);

		PT_ASSERT_STR_EQ(as_str(x), as_str(y));
		PT_ASSERT(as_str(x) isnt as_str(y));
		PT_ASSERT(x isnt y);

		delete(x);
		delete(y);
	}

	PT_TEST(test_eq) {
		PT_ASSERT(  eq($(Int, 1), $(Int, 1 )) );
		PT_ASSERT( neq($(Int, 2), $(Int, 20)) );
		PT_ASSERT(  eq($(String, "Hello"), $(String, "Hello")) );
		PT_ASSERT( neq($(String, "Hello"), $(String, "There")) );

		var tab1 = new(Table, String, Int);
		var tab2 = new(Table, String, Int);

		PT_ASSERT(eq(tab1, tab2));
		put(tab1, $(String, "apple"), $(Int, 10));
		PT_ASSERT(neq(tab1, tab2));
		put(tab2, $(String, "apple"), $(Int, 10));
		PT_ASSERT(eq(tab1, tab2));
		put(tab1, $(String, "pear"), $(Int, 20));
		put(tab2, $(String, "pear"), $(Int, 30));
		PT_ASSERT(neq(tab1, tab2));
		put(tab1, $(String, "pear"), $(Int, 30));
		PT_ASSERT(eq(tab1, tab2));
		put(tab2, $(String, "banana"), $(Int, 10));
		PT_ASSERT(neq(tab1, tab2));

		delete(tab1);
		delete(tab2);
	}
}
