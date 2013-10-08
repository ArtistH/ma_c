/*
** $Id: llimits.h,v 1.69.1.1 2007/12/27 13:02:25 roberto Exp $
** Limits, basic types, and some other `installation-dependent' definitions
** See Copyright Notice in lua.h
*/

#ifndef llimits_h
#define llimits_h


#include <limits.h>
#include <stddef.h>

#include "lua.h"

typedef LUAI_UINT32 lu_int32;
typedef LUAI_UMEM lu_mem;
typedef LUAI_MEM l_mem;

/* chars used as small naturals (so that `char` is reserved for characters) */
typedef unsigned char lu_byte;

#define MAX_SIZET	 ((size_t)(~(size_t)0)-2)
#define MAX_LUMEM	 ((lu_mem)(~(lu_mem)0)-2)
#define MAX_INT		(INT_MAX-2)  /* maximum value of an int (-2 for safety) */

#endif
