/*
** $Id: lvm.c,v 2.63.1.5 2011/08/17 20:43:11 roberto Exp $
** Lua virtual machine
** See Copyright Notice in lua.h
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define lvm_c
#define LUA_CORE

#include "lua.h"

#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lgc.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"
#include "lvm.h"



/* limit for table tag-method chains (to avoid loops) */
#define MAXTAGLOOP	100


const TValue *luaV_tonumber (const TValue *obj, TValue *n) {
	lua_Number num;
	if (ttisnumber(obj)) {
		return obj;
	}
	if (ttisstring(obj) && luaO_str2d(svalue(obj), &num)) {
		setnvalue(n, num);
		return n;
	} else {
		return NULL;
	}
}


int luaV_tostring (lua_State *L, StkId obj) {
	if (!ttisnumber(obj)) {
		return 0;
	} else {
		char s[LUAI_MAXNUMBER2STR];
		lua_Number n = nvalue(obj);
		lua_number2str(s, n);
		setsvalue2s(L, obj, luaS_new(L, s));
		return 1;
	}
}
