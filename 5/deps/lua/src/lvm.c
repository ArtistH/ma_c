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


static void traceexec (lua_State *L, const Instruction *pc) {
	lu_byte mask = L->hookmask;
	const Instruction *oldpc = L->savedpc;
	L->savedpc = pc;
	if ((mask & LUA_MASKCOUNT) && L->hookcount == 0) {
		resethookcount(L);
		luaD_callhook(L, LUA_HOOKCOUNT, -1);
	}
	if (mask & LUA_MASKLINE) {
		Proto *p = ci_func(L->ci)->l.p;
		int npc = pcRel(pc, p);
		int newline = getline(p, npc);
		/* call linehook when enter a new function, when jump back (loop),
		 * or when enter a new line. */
		if (npc == 0 || pc <= oldpc || newline != getline(p, pcRel(oldpc, p)))
			luaD_callhook(L, LUA_HOOKLINE, newline);
	}
}


static void callTMres (lua_State *L, StkId res, const TValue *f,
					   const TValue *p1, const TValue *p2) {
	ptrdiff_t result = savestack(L, res);
	setobj2s(L, L->top, f);  /* push function */
	setobj2s(L, L->top+1, p1);  /* 1st argument */
	setobj2s(L, L->top+2, p2);  /* 2nd argument */
	luaD_checkstack(L, 3);
	L->top += 3;
	luaD_call(L, L->top - 3, 1);
	res = restorestack(L, result);
	L->top--;
	setobj2s(L, res, L->top);
}



static void callTM (lua_State *L, const TValue *f, const TValue *p1,
					const TValue *p2, const TValue *p3) {
	setobj2s(L, L->top, f);  /* push function */
	setobj2s(L, L->top+1, p1);  /* 1st argument */
	setobj2s(L, L->top+2, p2);  /* 2nd argument */
	setobj2s(L, L->top+3, p3);  /* 3th argument */
	luaD_checkstack(L, 4);
	L->top += 4;
	luaD_call(L, L->top - 4, 0);
}


void luaV_gettable (lua_State *L, const TValue *t, TValue *key, StkId val) {
	int loop;
	for (loop = 0; loop < MAXTAGLOOP; loop++) {
		const TValue *tm;
		if (ttistable(t)) {  /* `t' is a table? */
			Table *h = hvalue(t);
			const TValue *res = luaH_get(h, key); /* do a primitive get */
			if (!ttisnil(res) ||  /* result is no nil? */
				(tm = fasttm(L, h->metatable, TM_INDEX)) == NULL) {
				/* or no TM? */
				setobj2s(L, val, res);
				return;
			}
			/* else will try the tag method */
		} else if (ttisnil(tm = luaT_gettmbyobj(L, t, TM_INDEX))) {
			luaG_typeerror(L, t, "index");
		}
		if (ttisfunction(tm)) {
			callTMres(L, val, tm, t, key);
			return;
		}
		t = tm;  /* else repeat with `tm' */
	}
	luaG_runerror(L, "loop in gettable");
}


void luaV_settable (lua_State *L, const TValue *t, TValue *key, StkId val) {
	int loop;
	TValue temp;
	for (loop = 0; loop < MAXTAGLOOP; loop++) {
		const TValue *tm;
		if (ttistable(t)) {  /* `t' is a table? */
			Table *h = hvalue(t);
			TValue *oldval = luaH_set(L, h, key); /* do a primitive set */
			if (!ttisnil(oldval) ||  /* result is no nil? */
				(tm = fasttm(L, h->metatable, TM_NEWINDEX)) == NULL) {
				/* or no TM? */
				setobj2t(L, oldval, val);
				h->flags = 0;
				luaC_barriert(L, h, val);
				return;
			}
			/* else will try the tag method */
		} else if (ttisnil(tm = luaT_gettmbyobj(L, t, TM_NEWINDEX))) {
			luaG_typeerror(L, t, "index");
		}
		if (ttisfunction(tm)) {
			callTM(L, tm, t, key, val);
			return;
		}
		/* else repeat with `tm' */
		setobj(L, &temp, tm);  /* avoid pointing inside table (may rehash) */
		t = &temp;
	}
	luaG_runerror(L, "loop in settable");
}
