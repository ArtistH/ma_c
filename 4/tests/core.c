
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

	PT_TEST(test_ord) {
		PT_ASSERT(gt($(Int, 15), $(Int, 3)));
		PT_ASSERT(lt($(Int, 70), $(Int, 81)));
		PT_ASSERT(ge($(Int, 71), $(Int, 71)));
		PT_ASSERT(ge($(Int, 78), $(Int, 71)));
		PT_ASSERT(le($(Int, 32), $(Int, 32)));
		PT_ASSERT(le($(Int, 21), $(Int, 32)));
	}

	PT_TEST(test_hash) {
		long x = hash($(Int, 1));
		long y = hash($(Int, 123));

		PT_ASSERT(x is 1);
		PT_ASSERT(y is 123);
	}

	PT_TEST(test_collection_list) {

		var x = new(List, $(Int, 1), $(Real, 2.0), $(String, "Hello"));

		PT_ASSERT(x);
		PT_ASSERT(len(x) is 3);
		PT_ASSERT(contains(x, $(Int, 1)));
		PT_ASSERT(contains(x, $(Real, 2.0)));
		PT_ASSERT(contains(x, $(String, "Hello")));

		discard(x, $(Real, 2.0));

		PT_ASSERT(x);
		PT_ASSERT(len(x) is 2);
		PT_ASSERT(contains(x, $(Int, 1)));
		PT_ASSERT(contains(x, $(String, "Hello")));

		clear(x);

		PT_ASSERT(x);
		PT_ASSERT(len(x) is 0);
		PT_ASSERT(is_empty(x));

		delete(x);
	}

	PT_TEST(test_collection_array) {

		var y = new(Array, Real, $(Real, 5.2), $(Real, 7.1), $(Real, 2.2), $(Real, 1.1));

		PT_ASSERT(y);
		PT_ASSERT(len(y) is 4);
		PT_ASSERT(contains(y, $(Real, 5.2)));
		PT_ASSERT(contains(y, $(Real, 7.1)));
		PT_ASSERT(contains(y, $(Real, 2.2)));
		PT_ASSERT(contains(y, $(Real, 1.1)));

		sort(y);

		PT_ASSERT(eq(at(y, 0), $(Real, 1.1)));
		PT_ASSERT(eq(at(y, 1), $(Real, 2.2)));
		PT_ASSERT(eq(at(y, 2), $(Real, 5.2)));
		PT_ASSERT(eq(at(y, 3), $(Real, 7.1)));

		var maxval = maximum(y);
		var minval = minimum(y);

		PT_ASSERT(eq(maxval, $(Real, 7.1)));
		PT_ASSERT(eq(minval, $(Real, 1.1)));

		delete(y);
	}

	PT_TEST(test_collection_list_sort) {

		var z = new(List, $(Real, 5.2), $(Real, 7.1), $(Real, 2.2), $(Real, 1.1));

		sort(z);

		PT_ASSERT(eq(at(z, 0), $(Real, 1.1)));
		PT_ASSERT(eq(at(z, 1), $(Real, 2.2)));
		PT_ASSERT(eq(at(z, 2), $(Real, 5.2)));
		PT_ASSERT(eq(at(z, 3), $(Real, 7.1)));

		delete(z);

		var w = new(List, $(Int, 135), $(Int, 11), $(Int, 254), $(Int, 123213), $(Int, 22), $(Int, 1));

		sort(w);

		PT_ASSERT(eq(at(w, 0), $(Int, 1)));
		PT_ASSERT(eq(at(w, 1), $(Int, 11)));
		PT_ASSERT(eq(at(w, 2), $(Int, 22)));
		PT_ASSERT(eq(at(w, 3), $(Int, 135)));
		PT_ASSERT(eq(at(w, 4), $(Int, 254)));
		PT_ASSERT(eq(at(w, 5), $(Int, 123213)));

		delete(w);
	}
/* 
	PT_TEST(test_collection_map) {

		var map1 = new(Map);
		var map2 = new(Map);

		PT_ASSERT(eq(map1, map2));

		put(map1, $(String, "key"), $(String, "val"));
		PT_ASSERT(neq(map1, map2));

		put(map2, $(String, "key"), $(String, "val"));
		PT_ASSERT(eq(map1, map2));

		put(map1, $(String, "newkey"), $(Int, 10));
		put(map2, $(String, "newkey"), $(String, "newval"));
		PT_ASSERT(neq(map1, map2));

		put(map1, $(String, "newkey"), $(String, "newval"));
		PT_ASSERT(eq(map1, map2));

		put(map2, $(String, "difkey"), $(Int, 5));
		PT_ASSERT(neq(map1, map2));

		delete(map1);
		delete(map2);

	} */
}
