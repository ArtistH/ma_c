/*
** == Tree ==
**
**  Container that stores keys and values
**
**  + Allocates space for key and value objects
**  + Key objects require "Ord" and "Eq" 
**  + Uses "Assign" to set data contents
*/

#ifndef CelloTree_h
#define CelloTree_h

#include "Prelude.h"
#include "Type.h"

global var Tree;

var Tree_New(var self, var_list vl);
var Tree_Delete(var self);
size_t Tree_Size(void);

void Tree_Assign(var self, var obj);
var Tree_Copy(var self);

var Tree_Eq(var self, var obj);

int Tree_Len(var self);
void Tree_Clear(var self);
var Tree_Contains(var self, var key);
void Tree_Discard(var self, var key);

var Tree_Get(var self, var key);
void Tree_Put(var self, var key, var val);

var Tree_Iter_Start(var self);
var Tree_Iter_End(var self);
var Tree_Iter_Next(var self, var curr);

int Tree_Show(var self, var output, int pos);

instance(Tree, New) = {
Tree_New, Tree_Delete, Tree_Size};

instance(Tree, Assign) = {
Tree_Assign};

instance(Tree, Copy) = {
Tree_Copy};

instance(Tree, Eq) = {
Tree_Eq};

instance(Tree, Collection) = {
Tree_Len, Tree_Clear, Tree_Contains, Tree_Discard};

instance(Tree, Dict) = {
Tree_Get, Tree_Put};

instance(Tree, Iter) = {
Tree_Iter_Start, Tree_Iter_End, Tree_Iter_Next};

instance(Tree, Show) = {
Tree_Show, NULL};

#endif
