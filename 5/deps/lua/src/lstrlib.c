/*
** $Id: lstrlib.c,v 1.132.1.5 2010/05/14 15:34:19 roberto Exp $
** Standard library for string operations and pattern-matching
** See Copyright Notice in lua.h
*/


#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define lstrlib_c
#define LUA_LIB

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


/* macro to `unsign` a character */
#define uchar(c)		((unsigned char)(c))



static int str_len (lua_State *L) {
	size_t l;
	luaL_checklstring(L, 1, &l);
	lua_pushinteger(L, l);
	return 1;
}


static ptrdiff_t posrelat (ptrdiff_t pos, size_t len) {
	/* relative string position: negative means back from end */
	if (pos < 0) {
		pos += (ptrdiff_t)len + 1;
	}
	return (pos >= 0) ? pos : 0;
}


static int str_sub (lua_State *L) {
	size_t l;
	const char *s = luaL_checklstring(L, 1, &l);
	ptrdiff_t start = posrelat(luaL_checkinteger(L, 2), l);
	ptrdiff_t end = posrelat(luaL_optinteger(L, 3, -1), l);
	/* start = (start < 1) ? 1 : start; */
	/* end = (end > (ptrdiff_t)l) ? (ptrdiff_t)l : end; */
	if (start < 1) {
		start = 1;
	}
	if (end > (ptrdiff_t)l) {
		end = (ptrdiff_t)l;
	}
	if (start <= end) {
		lua_pushlstring(L, s+start-1, end-start+1);
	}
	return 1;
}

static int str_reverse (lua_State *L) {
	size_t l;
	luaL_Buffer b;
	const char *s = luaL_checklstring(L, 1, &l);
	luaL_buffinit(L, &b);
	while (l--) {
		luaL_addchar(&b, s[l]);
	}
	luaL_pushresult(&b);
	return 1;
}


static int str_lower (lua_State *L) {
	size_t l;
	size_t i;
	luaL_Buffer b;
	const char *s = luaL_checklstring(L, 1, &l);
	luaL_buffinit(L, &b);
	for (i = 0; i < l; i++) {
		luaL_addchar(&b, tolower(uchar(s[i])));
	}
	luaL_pushresult(&b);
	return 1;
}


static int str_upper (lua_State *L) {
	size_t l;
	size_t i;
	luaL_Buffer b;
	const char *s = luaL_checklstring(L, 1, &l);
	luaL_buffinit(L, &b);
	for (i = 0; i < l; i++) {
		luaL_addchar(&b, toupper(uchar(s[i])));
	}
	luaL_pushresult(&b);
	return 1;
}


static int str_rep (lua_State *L) {
	size_t l;
	luaL_Buffer b;
	const char *s = luaL_checklstring(L, 1, &l);
	int n = luaL_checkint(L, 2);
	luaL_buffinit(L, &b);
	while (n-- > 0) {
		luaL_addlstring(&b, s, l);
	}
	luaL_pushresult(&b);
	return 1;
}


static int str_byte (lua_State *L) {
	size_t l;
	const char *s = luaL_checklstring(L, 1, &l);
	ptrdiff_t posi = posrelat(luaL_optinteger(L, 2, 1), l);
	ptrdiff_t pose = posrelat(luaL_optinteger(L, 3, posi), l);
	int n;
	int i;
	if (posi <= 0) {
		posi = 1;
	}
	if ((size_t)pose > l) {
		pose = l;
	}
	if (posi > pose) {
		return 0;  /* empty interval; return no values */
	}
	n = (int)(pose - posi + 1);
	if (posi + n <= pose) {  /* overflow */
		luaL_error(L, "string slice too long");
	}
	luaL_checkstack(L, n, "string slice too long");
	for (i = 0; i < n; i++) {
		lua_pushinteger(L, uchar(s[posi+i-1]));
	}
	return n;
}


static int str_char (lua_State *L) {
	int n = lua_gettop(L);  /* number of arguments */
	int i;
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	for (i = 1; i <= n; i++) {
		int c = luaL_checkint(L, i);
		luaL_argcheck(L, uchar(c) == c, i, "invalid value");
		luaL_addchar(&b, uchar(c));
	}
	luaL_pushresult(&b);
	return 1;
}


static int writer (lua_State *L, const void* b, size_t size, void* B) {
	(void)L;
	luaL_addlstring((luaL_Buffer*)B, (const char *)b, size);
	return 0;
}


static int str_dump (lua_State *L) {
	luaL_Buffer b;
	luaL_checktype(L, 1, LUA_TFUNCTION);
	lua_settop(L, 1);
	luaL_buffinit(L, &b);
	if (lua_dump(L, writer, &b) != 0) {
		luaL_error(L, "unable to dump given function");
	}
	luaL_pushresult(&b);
	return 1;
}



/*
** {======================================================
** PATTERN MATCHING
** =======================================================
*/


#define CAP_UNFINISHED		(-1)
#define CAP_POSITION		(-2)

typedef struct MatchState {
	const char *src_init;  /* init of source string */
	const char *src_end;  /* end (`\0`) of source string */
	lua_State *L;
	int level;  /* total number of captures (finished ot unfinished) */
	struct {
		const char *init;
		ptrdiff_t len;
	} capture[LUA_MAXCAPTURES];
} MatchState;


#define L_ESC		'%'
#define SPECIALS	"^$*+?.([%-"


static int check_capture (MatchState *ms, int l) {
	l -= '1';
	if (l < 0 || l >= ms->level || ms->capture[l].len == CAP_UNFINISHED) {
		return luaL_error(ms->L, "invalid capture index");
	}
	return l;
}


static int capture_to_close (MatchState *ms) {
	int level = ms->level;
	for (level--; level >= 0; level--) {
		if (ms->capture[level].len == CAP_UNFINISHED) {
			return level;
		}
	}
	return luaL_error(ms->L, "invalid pattern capture");
}


static const char *classend (MatchState *ms, const char *p) {
	switch (*p++) {
	case L_ESC: {
		if (*p == '\0') {
			luaL_error(ms->L, "malformed pattern (ends with )" LUA_QL("%%") ")");
		}
		return p+1;
				}
	case '[': {
		if (*p == '^') {
			p++;
		}
		do {
			if (*p == '\0') {
				luaL_error(ms->L, "malformed pattern (missing" LUA_QL("]") ")");
			}
			if (*(p++) == L_ESC && *p != '\0') {
				p++;  /* skip escapes (e.g. '%]') */
			}
		} while (*p != ']');
		return p+1;
			  }
	default: {
		return p;
			 }
	}
}


static int match_class (int c, int cl) {
	int res;
	switch (tolower(cl)) {
	case 'a': res = isalpha(c); break;
	case 'c' : res = iscntrl(c); break;
	case 'd' : res = isdigit(c); break;
	case 'l' : res = islower(c); break;
	case 'p' : res = ispunct(c); break;
	case 's' : res = isspace(c); break;
	case 'u' : res = isupper(c); break;
	case 'w' : res = isalnum(c); break;
	case 'x' : res = isxdigit(c); break;
	case 'z' : res = (c == 0); break;
	default: return (cl == c);
	}
	return (islower(cl) ? res : !res);
}


static int matchbracketclass (int c, const char *p, const char *ec) {
	int sig = 1;
	if (*(p+1) == '^') {
		sig = 0;
		p++;  /* skip the '^' */
	}
	while (++p < ec) {
		if (*p == L_ESC) {
			p++;
			if (match_class(c, uchar(*p))) {
				return sig;
			}
		} else if (*(p+1) == '-' && (p+2 < ec)) {
			p += 2;
			if (uchar(*(p-2)) <= c && c <= uchar(*p)) {
				return sig;
			}
		} else if (uchar(*p) == c) {
			return sig;
		}
	}
	return !sig;
}


static int singlematch (int c, const char *p, const char *ep) {
	switch (*p) {
	case '.': return 1;  /* matches any char */
	case L_ESC: return match_class(c, uchar(*(p+1)));
	case '[': return matchbracketclass(c, p, ep-1);
	default: return (uchar(*p) == c);
	}
}


static const char *match (MatchState *ms, const char *s, const char *p);
