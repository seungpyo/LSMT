#include "LSMT.h"

struct component * component_create ( size_t capacity ) {
	struct component * component;
	component = malloc(sizeof(struct component));
	component->nument = 0;
	component->capacity = capacity;
	component->keys = malloc(sizeof(uint32_t) * capacity);
	component->entrefs = calloc(capacity, sizeof(struct entry *));
	return component;
}

void component_free (struct component * component) {
	free(component->keys);
	free(component->entrefs);
	free(component);
}

uint32_t component_find_idx (struct component * component, uint32_t key) {
	// This function does NOT check the index overflow
	int idx;
	for (idx = 0; idx < component->nument; idx++) {
		if (component->keys[idx] == key) {
			return idx;
		}
	}
	return idx;
}


void component_insert (struct component * component, 
						uint32_t key, struct entry * entry) {
	uint32_t idx = component_find_idx(component, key);
	if (idx >= component->capacity) {
		// Current component is full; rolling merge should be done.
		printf("Current component is full, but rolling merge NYI\n");
		return;
	}

	if (idx < component->nument) {
		// Duplicate key found; Overwrite.
		// Previous entry is freed
		free(component->entrefs[idx]);
	}
	// Append.
	component->keys[idx] = key;
	component->entrefs[idx] = entry;

	component->nument++;
}

void component_expr (struct component * component) {

	printf("Component @ %p\n", component);
	printf("* Number of entries : %zu\n", component->nument);
	printf("* Capacity : %zu\n", component->capacity);
	printf("* (Key, Value dump)\n");
	for (int i = 0; i < component->nument; i++) {
		printf("** [%u] : %s\n", 
				component->keys[i], component->entrefs[i]->bytes);
	}
	printf("WARNING: bytes after NULL characters may be omitted in dump.\n");

}

struct entry * component_find (struct component * component, uint32_t key) {
	uint32_t idx = component_find_idx(component, key);
	return idx < component->capacity ? component->entrefs[idx] : NULL;
}

void component_delete (struct component * component, uint32_t key) {
	uint32_t idx = component_find_idx(component, key);
	if (idx >= component->nument) {
		// Such key does not exist
		return;
	}
	entry_free(component->entrefs[idx]);
	for (int i = idx; i < component->nument-1; i++) {
		component->keys[i] = component->keys[i+1];
		component->entrefs[i] = component->entrefs[i+1];
	}
	component->keys[component->nument-1] = 0;
	component->entrefs[component->nument-1] = NULL;
	component->nument--;
}


int main() {
	#define CAPACITY 10
	#define DATALEN 8
	struct component *c = component_create(CAPACITY);
	
	printf("Overflow Test\n");
	struct entry * e[CAPACITY+5];
	char data[DATALEN];
	for (int i = 0; i < CAPACITY+5 ; i++) {
		memset(data, 'A'+i, DATALEN);
		// Length should also include NULL character.
		e[i] = entry_create(DATALEN, data);
		component_insert(c, i, e[i]);
	}
	component_expr(c);
	
	printf("Search Test\n");
	struct entry * res;
	for (int i = 0; i < CAPACITY + 10; i++) {
		res = component_find(c, i);
		if (res) {
			printf("For key %u, found %s\n", i, res->bytes);
		} else {
			printf("No entry found for key %u\n", i);
		}
	}
	component_expr(c);

	printf("Delete Test\n");
	for (int i = 0; i < CAPACITY - 5; i++) {
		component_delete(c, i);
	}
	component_expr(c);

	printf("Delete non-existing key test\n");
	component_delete(c, 0);
	component_expr(c);

	
	printf("Free Test\n");
	component_free(c);
	printf("End of all tests.\n");
	return 0;
}
