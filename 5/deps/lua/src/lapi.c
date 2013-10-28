/*
** $Id: lapi.c,v 2.55.1.5 2008/07/04 18:41:18 roberto Exp $
** Lua API
** See Copyright Notice in lua.h
*/


#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>

#define lapi_c
#define LUA_CORE

#include "lua.h"

#include "lapi.h"
#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lgc.h"
#include "lmem.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"
#include "lundump.h"
#include "lvm.h"



const char lua_ident[] =
"$Lua: " LUA_RELEASE " " LUA_COPYRIGHT " $\n"
"$Authors: " LUA_AUTHORS " $\n"
"$URL: www.lua.org $\n";



#define api_checknelems(L, n)	api_check(L, (n) <= (L->top - L-base))

#define api_checkvalidindex(L, i)	api_check(L, (i) != luaO_nilobject)

#define api_incr_top(L)		{api_check(L, L->top < L->ci->top); L->top++;}



static TValue *index2adr (lua_State *L, int idx) {
	if (idx > 0) {
		TValue *o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) {
			return cast(TValue *, luaO_nilobject);
		} else {
			return o;
		}
	} else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	} else switch (idx) {  /* pseudo-indices */
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
							   Closure *func = curr_func(L);
							   sethvalue(L, &L->env, func->c.env);
							   return &L->env;
						   }
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
				 Closure *func = curr_func(L);
				 idx = LUA_GLOBALSINDEX - idx;
				 return (idx <= func->c.nupvalues)
					 ? &func->c.upvalue[idx-1]
					 : cast(TValue *, luaO_nilobject);
			 }
	}
}


static Table *getcurrenv (lua_State *L) {
	if (L->ci == L->base_ci) {  /* no enclosing function? */
		return hvalue(gt(L));  /* use global table as environment */
	} else {
		Closure *func = curr_func(L);
		return func->c.env;
	}
}


void luaA_pushobject (lua_State *L, const TValue *o) {
	setobj2s(L, L->top, o);
	api_incr_top(L);
}


LUA_API int lua_checkstack (lua_State *L, int size) {
	int res = 1;
	lua_lock(L);
	if (size > LUAI_MAXCSTACK || (L->top - L->base + size) > LUAI_MAXCSTACK) {
		res = 0;  /* stack overflow */
	} else if (size > 0) {
		luaD_checkstack(L, size);
		if (L->ci->top < L->top + size) {
			L->ci->top = L->top + size;
		}
	}
	lua_unlock(L);
	return res;
}


LUA_API void lua_xmove (lua_State *from, lua_State *to, int n) {
	int i;
	if (from == to) {
		return;
	}
	lua_lock(to);
	api_checknelems(from, n);
	api_check(from, G(from) == G(to));
	api_check(from, to->ci->top - to->top >= n);
	from->top -= n;
	for (i = 0; i < n; i++) {
		setobj2s(to, to->top++, from->top + i);
	}
	lua_unlock(to);
}


LUA_API void lua_setlevel (lua_State *from, lua_State *to) {
	to->nCcalls = from->nCcalls;
}


LUA_API lua_CFunction lua_atpanic (lua_State *L, lua_CFunction panicf) {
	lua_CFunction old;
	lua_lock(L);
	old = G(L)->panic;
	G(L)->panic = panicf;
	lua_unlock(L);
	return old;
}


LUA_API lua_State *lua_newthread (lua_State *L) {
	lua_State *L1;
	lua_lock(L);
	luaC_checkGC(L);
	L1 = luaE_newthread(L);
	setthvalue(L, L->top, L1);
	api_incr_top(L);
	lua_unlock(L);
	luai_userstatethread(L, L1);
	return L1;
}


/*
** basic stack manipulation
*/


LUA_API int lua_gettop (lua_State *L) {
	return cast_int(L->top - L->base);
}


LUA_API void lua_settop (lua_State *L, int idx) {
	lua_lock(L);
	if (idx >= 0) {
		api_check(L, idx <= L->stack_last - L->base);
		while (L->top < L->base + idx) {
			setnilvalue(L->top++);
		}
	} else {
		api_check(L, -(idx+1) <= (L->top - L->base));
		L->top += idx+1;  /* `subtract' index (index is negative) */
	}
	lua_unlock(L);
}


LUA_API void lua_remove (lua_State *L, int idx) {
	StkId p;
	lua_lock(L);
	p = index2adr(L, idx);
	api_checkvalidindex(L, p);
	while (++p < L->top) {
		setobj2s(L, p-1, p);
	}
	L->top--;
	lua_unlock(L);
}


LUA_API void lua_insert (lua_State *L, int idx) {
	StkId p;
	StkId q;
	p = index2adr(L, idx);
	api_checkvalidindex(L, p);
	for (q = L->top; q > p; q--) {
		setobj2s(L, q, q-1);
	}
	setobj2s(L, p, L->top);
	lua_unlock(L);
}


LUA_API void lua_replace (lua_State *L, int idx) {
	StkId o;
	lua_lock(L);
	/* explicit test for incompatible code */
	if (idx == LUA_ENVIRONINDEX && L->ci == L->base_ci) {
		luaG_runerror(L, "no calling environment");
	}
	api_checknelems(L, 1);
	o = index2adr(L, idx);
	api_checkvalidindex(L, o);
	if (idx == LUA_ENVIRONINDEX) {
		Closure *func = curr_func(L);
		api_check(L, ttistable(L->top - 1));
		func->c.env = hvalue(L->top - 1);
		luaC_barrier(L, func, L->top - 1);
	} else {
		setobj(L, o, L->top - 1);
		if (idx < LUA_GLOBALSINDEX) {
			luaC_barrier(L, curr_func(L), L->top - 1);
		}
	}
	L->top--;
	lua_unlock(L);
}


LUA_API void lua_pushvalue (lua_State *L, int idx) {
	lua_lock(L);
	setobj2s(L, L->top, index2adr(L, idx));
	api_incr_top(L);
	lua_unlock(L);
}



/*
** access functions (stack -> C)
*/



