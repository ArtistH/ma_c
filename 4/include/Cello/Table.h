/*
** == Table ==
**
**  Container that stores keys and values
**
**  + Allocates space for key and value objects
**  + Key objects require "Hash" and "Eq" 
**  + Uses "Assign" to set data contents
*/

#ifndef CelloTable_h
#define CelloTable_h

#include "Prelude.h"
#include "Type.h"

global var Table;

var Table_New(var self, var_list vl);
var Table_Delete(var self);
size_t Table_Size(void);
void Table_Assign(var self, var obj);
var Table_Copy(var self);

var Table_Eq(var self, var obj);

int Table_Len(var self);
void Table_Clear(var self);
var Table_Contains(var self, var key);
void Table_Discard(var self, var key);

var Table_Get(var self, var key);
void Table_Put(var self, var key, var val);

var Table_Iter_Start(var self);
var Table_Iter_End(var self);
var Table_Iter_Next(var self, var curr);

int Table_Show(var self, var output, int pos);

instance(Table, New) = {
Table_New, Table_Delete, Table_Size};

instance(Table, Assign) = {
Table_Assign};

instance(Table, Copy) = {
Table_Copy};

instance(Table, Eq) = {
Table_Eq};

instance(Table, Collection) = {
Table_Len, Table_Clear, Table_Contains, Table_Discard};

instance(Table, Dict) = {
Table_Get, Table_Put};

instance(Table, Iter) = {
Table_Iter_Start, Table_Iter_End, Table_Iter_Next};

instance(Table, Show) = {
Table_Show, NULL};

#endif
