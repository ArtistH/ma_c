
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
	}

}
