#include "Cello/Prelude.h"

#include "Cello/Type.h"
#include "Cello/Bool.h"
#include "Cello/None.h"
#include "Cello/Exception.h"
#include "Cello/File.h"
#include "Cello/String.h"
#include "Cello/Number.h"
#include "Cello/Char.h"
#include "Cello/Reference.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var Undefined = Singleton(Undefined);

/*
** The type_of a Type object is just "Type" again.
** But because "Type" is extern it isn't a constant expression.
** This means it cannot be set at compile time.
**
** So by convention at compile time the type_of a Type object is set to NULL.
** So if we access a struct and it tells us NULL is the type, assume "Type".
*/

var type_of(var self) {
	/* Test against Builtins */
	if (self is Undefined) return throw(ValueError, "Received 'Undefined' as value to 'type_of'");
	if (self is True) return Bool;
	if (self is False) return Bool;

	/* Use first entry in struct */
	var entry = ((ObjectData*)self)->type;
	if (entry is NULL) {
		return Type;
	} else {
		return entry;
	}
}

/* Allocate space for type, set type entry */
var allocate(var type) {
	type = cast(type, Type);

	New* inew = type_class(type, New);

	var self;
	if (inew->size <= sizeof(ObjectData)) {
		self = NULL;
	} else {
		self = calloc(1, inew->size);
		if (self == NULL) {
			throw(OutOfMemoryError,
				  "Cannot create new '%s', out of memory!", type);
		}
		((ObjectData*)self)->type = type;
	}

	return self;
}

void deallocate(var obj) {
	free(obj);
}

var new(var type, ...) {
	var self = allocate(type);

	New* inew = type_class(type, New);
	if (inew->construct) {
		va_list args;
		va_start(args, type);
		self = inew->construct(self, &args);
		va_end(args);
	}

	return self;
}

void delete(var self) {
	New* inew = type_class(type_of(self), New);

	if (inew->destruct) {
		self = inew->destruct(self);
	}

	deallocate(self);
}

var construct(var self, ...) {
	va_list args;
	va_start(args, self);
	self = type_class_method(type_of(self), New, construct, self, &args);
	va_end(args);
	return self;
}

var destruct(var self) {
	return type_class_method(type_of(self), New, destruct, self);
}

void assign(var self, var obj) {
	type_class_method(type_of(self), Assign, assign, self, obj);
}

var copy(var self) {
	return type_class_method(type_of(self), Copy, copy, self);
}

var eq(var lhs, var rhs) {
	if (not type_implements(type_of(lhs), Eq)) {
		return (var)(intptr_t)(lhs == rhs);
	} else {
		return type_class_method(type_of(lhs), Eq, eq, lhs, rhs);
	}
}

var neq(var lhs, var rhs) {
	return (var)(intptr_t)(not eq(lhs, rhs));
}
