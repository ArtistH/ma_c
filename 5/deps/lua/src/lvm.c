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


static int call_binTM (lua_State *L, const TValue *p1, const TValue *p2,
					   StkId res, TMS event) {
	const TValue *tm = luaT_gettmbyobj(L, p1, event);  /* try first operand */
	if (ttisnil(tm)) {
		tm = luaT_gettmbyobj(L, p2, event);  /* try second operand */
	}
	if (ttisnil(tm)) {
		return 0;
	}
	callTMres(L, res, tm, p1, p2);
	return 1;
}

static const TValue *get_compTM (lua_State *L, Table *mt1, Table *mt2,
								 TMS event) {
	const TValue *tm1 = fasttm(L, mt1, event);
	const TValue *tm2;
	if (tm1 == NULL) return NULL;  /* no metamethod */
	if (mt1 == mt2) return tm1;  /* same metatables => same metamethods */
	tm2 = fasttm(L, mt2, event);
	if (tm1 == NULL) return NULL;  /* no metamethod */
	if (luaO_rawequalObj(tm1, tm2))  return tm1;  /* same metamethods? */
	return NULL;
}


static int call_orderTM (lua_State *L, const TValue *p1, const TValue *p2,
						 TMS event) {
	const TValue *tm1 = luaT_gettmbyobj(L, p1, event);
	const TValue *tm2;
	if (ttisnil(tm1)) return -1;  /* no metamethod? */
	tm2 = luaT_gettmbyobj(L, p2, event);
	if (!luaO_rawequalObj(tm1, tm2))  /* different metamethods? */
		return -1;
	callTMres(L, L->top, tm1, p1, p2);
	return !l_isfalse(L->top);
}


static int l_strcmp (const TString *ls, const TString *rs) {
	const char *l = getstr(ls);
	size_t ll = ls->tsv.len;
	const char *r = getstr(rs);
	size_t lr = rs->tsv.len;
	for (;;) {
		int temp = strcoll(l, r);
		if (temp != 0) {
			return temp;
		} else {  /* strings are equal up to a `\0' */
			size_t len = strlen(l);  /* index of first `\0' in both strings */
			if (len == lr) {  /* r is finished? */
				return (len == ll) ? 0 : 1;
			} else if (len == ll) {  /* l is finished? */
				/* l is smaller than r (because r is not finished) */
				return -1;
			}
			/* both strings longer than `len';
			 * go on comparing (after the `\0') */
			len++;
			l += len; ll -= len; r += len; lr -= len;
		}
	}
}


int luaV_lessthan (lua_State *L, const TValue *l, const TValue *r) {
	int res;
	if (ttype(l) != ttype(r)) {
		return luaG_ordererror(L, l, r);
	} else if (ttisnumber(l)) {
		return luai_numlt(nvalue(l), nvalue(r));
	} else if (ttisstring(l)) {
		return l_strcmp(rawtsvalue(l), rawtsvalue(r)) < 0;
	} else if ((res = call_orderTM(L, l, r, TM_LT)) != -1) {
		return res;
	}
	return luaG_ordererror(L, l, r);
}


static int lessequal (lua_State *L, const TValue *l, const TValue *r) {
	int res;
	if (ttype(l) != ttype(r)) {
		return luaG_ordererror(L, l, r);
	} else if (ttisnumber(l)) {
		return luai_numle(nvalue(l), nvalue(r));
	} else if (ttisstring(l)) {
		return l_strcmp(rawtsvalue(l), rawtsvalue(r)) <= 0;
	} else if ((res = call_orderTM(L, l, r, TM_LE)) != -1) {
		/* first try `le' */
		return res;
	} else if ((res = call_orderTM(L, l, r, TM_LT)) != -1) {
		/* else try `lt' */
		return !res;
	}
	return luaG_ordererror(L, l, r);
}


int luaV_equalval (lua_State *L, const TValue *t1, const TValue *t2) {
	const TValue *tm;
	lua_assert(ttype(t1) == ttype(t2));
	switch (ttype(t1)) {
	case LUA_TNIL: return 1;
	case LUA_TNUMBER: return luai_numeq(nvalue(t1), nvalue(t2));
	case LUA_TBOOLEAN: return bvalue(t1) == bvalue(t2);  /* true must be 1 */
	case LUA_TLIGHTUSERDATA: return pvalue(t1) == pvalue(t2);
	case LUA_TUSERDATA: {
		if (uvalue(t1) == uvalue(t2)) return 1;
		tm = get_compTM(L, uvalue(t1)->metatable, uvalue(t2)->metatable,TM_EQ);
		break;  /* will try TM */
						}
	case LUA_TTABLE: {
		if (hvalue(t1) == hvalue(t2)) return 1;
		tm = get_compTM(L, hvalue(t1)->metatable, hvalue(t2)->metatable,TM_EQ);
		break;  /* will try TM */
					 }
	default: return gcvalue(t1) == gcvalue(t2);
	}
	if (tm == NULL) return 0;  /* no TM? */
	callTMres(L, L->top, tm, t1, t2);  /* call TM */
	return !l_isfalse(L->top);
}
