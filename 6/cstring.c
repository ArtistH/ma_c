#include "cstring.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#define FORMAT_TEMP_SIZE 1024

#define INTERNING_POOL_SIZE 1024
// HASH_START_SIZE must be 2 pow
#define HASH_START_SIZE 16

struct string_node {
	struct cstring_data str;
	char buffer[CSTRING_INTERNING_SIZE];
	struct string_node * next;
};

struct string_pool {
	struct string_node node[INTERNING_POOL_SIZE];
};

struct string_interning {
	int lock;
	int size;
	struct string_node ** hash;
	struct string_pool * pool;
	int index;
	int total;
};

static struct string_interning S;

static inline void
LOCK() {
	while (__sync_lock_test_and_set(&(S.lock), 1)) {}
}

static inline void 
UNLOCK() {
	__sync_lock_release(&(S.lock));
}

static void
insert_node(struct string_node ** hash, int sz, struct string_node * n) {
	uint32_t h = n->str.hash_size;
	int index = h & (sz-1);
	n->next = hash[index];
	hash[index] = n;
}

static void
expand(struct string_interning * si) {
	int new_size = si->size * 2;
	if (new_size < HASH_START_SIZE) {
		new_size = HASH_START_SIZE;
	}
	assert(new_size > si->total);
	struct string_node ** new_hash = malloc(sizeof(struct string_node *) * new_size);
	memset(new_hash, 0, sizeof(struct string_node *) * new_size);
	int i;
	for (i = 0; i < si->size; i++) {
		struct string_node * node = si->hash[i];
		while (node) {
			struct string_node * tmp = node->next;
			insert_node(new_hash, new_size, node);
			node = tmp;
		}
	}
	free(si->hash);
	si->hash = new_hash;
	si->size = new_size;
}

static cstring
interning(struct string_interning * si, const char * cstr, size_t sz, uint32_t hash) {
	if (si->hash == NULL) {
		return NULL;
	}
	int index = (int)(hash & (si->size-1));
	struct string_node * n = si->hash[index];
	while (n) {
		if (n->str.hash_size == hash) {
			if (strcmp(n->str.cstr, cstr) == 0) {
				return &n->str;
			}
		}
		n = n->next;
	}
	// 80% (4/5) threshold
	if (si->total * 5 >= si->size * 4) {
		return NULL;
	}
	if (si->pool == NULL) {
		// need not free pool
		// todo: check memory alloc error
		si->pool = malloc(sizeof(struct string_pool));
		assert(si->pool);
		si->index = 0;
	}
	n = &si->pool->node[si->index++];
	memcpy(n->buffer, cstr, sz);
	n->buffer[sz] = '\0';

	cstring cs = &n->str;
	cs->cstr = n->buffer;
	cs->hash_size = hash;
	cs->type = CSTRING_INTERNING;
	cs->ref = 0;

	n->next = si->hash[index];
	si->hash[index] = n;

	return cs;
}

static cstring
cstring_interning(const char * cstr, size_t sz, uint32_t hash) {
	cstring ret;
	LOCK();
	ret = interning(&S, cstr, sz, hash);
	if (ret == NULL) {
		expand(&S);
		ret = interning(&S, cstr, sz, hash);
	}
	UNLOCK();
	assert(ret);
	return ret;
}

static uint32_t
hash_blob(const char * buffer, size_t len) {
	const uint8_t * ptr = (const uint8_t *) buffer;
	size_t h = len;
	size_t step = (len>>5)+1;
	size_t i;
	for (i = len; i >= step; i -= step) {
		h = h ^ ((h<<5) + (h>>2) + ptr[i-1]);
	}
	if (h == 0) {
		return 1;
	} else {
		return h;
	}
}

void
cstring_free_persist(cstring s) {
	if (s->type == CSTRING_PERMANENT) {
		free(s);
	}
}

static cstring
cstring_clone(const char * cstr, size_t sz) {
	if (sz < CSTRING_INTERNING_SIZE) {
		return cstring_interning(cstr, sz, hash_blob(cstr, sz));
	}
	struct cstring_data * p = malloc(sizeof(struct cstring_data) + sz + 1);
	// todo: memory alloc error
	assert(p);
	void * ptr = (void *)(p + 1);
	p->cstr = ptr;
	p->type = 0;
	p->ref = 1;
	memcpy(ptr, cstr, sz);
	((char *)ptr)[sz] = '\0';
	p->hash_size = 0;
	return p;
}

cstring
cstring_persist(const char * cstr, size_t sz) {
	cstring s = cstring_clone(cstr, sz);
	if (s->type == 0) {
		s->type = CSTRING_PERMANENT;
		s->ref = 0;
	}
	return s;
}

cstring
cstring_grab(cstring s) {
	if (s->type & (CSTRING_PERMANENT | CSTRING_INTERNING)) {
		return s;
	}
	if (s->type == CSTRING_ONSTACK) {
		cstring tmp = cstring_clone(s->cstr, s->hash_size);
		return tmp;
	} else {
		if (s->ref == 0) {
			s->type = CSTRING_PERMANENT;
		} else {
			__sync_add_and_fetch(&s->ref, 1);
		}
		return s;
	}
}

void
cstring_release(cstring s){
	if (s->type != 0) {
		return;
	}
	if (s->type == 0) {
		return;
	}
	if (__sync_sub_and_fetch(&s->ref, 1) == 0) {
		free(s);
	}
}

uint32_t
cstring_hash(cstring s) {
	if (s->type == CSTRING_ONSTACK) {
		return hash_blob(s->cstr, s->hash_size);
	}
	if (s->hash_size == 0) {
		s->hash_size = hash_blob(s->cstr, strlen(s->cstr));
	}
	return s->hash_size;
}
