#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>

struct entry {
	size_t length;
	char * bytes;
};
struct entry * entry_create (size_t length, char * bytes);
void entry_free (struct entry * entry);

struct component {
	size_t nument; // Number of elements
	size_t capacity;
	uint32_t * keys;
	struct entry ** entrefs;
};

struct component * component_create ( size_t capacity );
void component_free (struct component * component);
uint32_t component_find_idx (struct component * component, uint32_t key);
void component_insert (struct component * component, 
						uint32_t key, struct entry * entry);
void component_expr (struct component * component);
struct entry * component_find (struct component * component, uint32_t key);
void component_delete (struct component * component, uint32_t key);

struct lsmt {
	struct component * components;
	size_t nument;
};

