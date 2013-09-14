#ifndef CelloModule_h
#define CelloModule_h

#ifdef _WIN32
  #undef data
  #undef in
  #include <windows.h>
  #define data typedef struct
  #define in ,
#else
  #include <dlfcn.h>
#endif


#include "Prelude.h"
#include "Function.h"
#include "Exception.h"

global var Module;

data {
  var type;
  char* name;
  
  #ifdef _WIN32
    HINSTANCE lib;
  #else
    void* lib;
  #endif
  
} ModuleData;

#define module(name) $(Module, name, NULL)

var Module_New(var self, var_list vl);
var Module_Delete(var self);
size_t Module_Size(void);

var Module_Copy(var self);
void Module_Assign(var self, var obj);

void Module_Enter(var self);
void Module_Exit(var self);

var Module_Get(var self, var k);

var Module_Call(var self, var args);

instance(Module, New) = { Module_New, Module_Delete, Module_Size };
instance(Module, Copy) = { Module_Copy };
instance(Module, Assign) = { Module_Assign };
instance(Module, With) = { Module_Enter, Module_Exit };
instance(Module, Dict) = { Module_Get, NULL };
instance(Module, Call) = { Module_Call };

#endif