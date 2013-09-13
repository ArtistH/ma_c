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
#include <stddef.h>
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

size_t size(var type) {
  return type_class_method(cast(type, Type), New, size);
}

var allocate(var type) {

  /* Allocate space for type, set type entry */
  type = cast(type, Type);
  
  var self;
  if (size(type) <= sizeof(ObjectData)) {
    self = NULL;
  } else {
    self = calloc(1, size(type));
    if (self == NULL) { throw(OutOfMemoryError, "Cannot create new '%s', out of memory!", type); }
    ((ObjectData*)self)->type = type;
  }
  
  return self;
}

void deallocate(var obj) {
	free(obj);
}

var new_vl(var type, var_list vl) { 
  
  var self = allocate(type);
  
  if (type_implements_method(type, New, construct_vl)) {
    self = type_class_method(type, New, construct_vl, self, vl);
  }
  
  return self;
}

void delete(var self) {
  
  if (type_implements_method(type_of(self), New, destruct)) {
    self = type_class_method(type_of(self), New, destruct, self);
  }
  
  deallocate(self);
}

var construct_vl(var self, var_list vl) {
  return type_class_method(type_of(self), New, construct_vl, self, vl);
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
    return bool_var(lhs == rhs);
  } else {
    return type_class_method(type_of(lhs), Eq, eq, lhs, rhs);
  }
  
}

var neq(var lhs, var rhs) {
  return bool_var(not eq(lhs, rhs));
}
