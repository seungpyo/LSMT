#include "LSMT.h"

struct entry * entry_create (size_t length, char * bytes) {
	struct entry * entry = malloc(sizeof(struct entry));
	if (!entry) {
		perror("entry_create failed to malloc entry");
		exit(EXIT_FAILURE);
	}
	entry->length = length;
	entry->bytes = malloc(sizeof(char) * length);
	if (!entry->bytes) {
		perror("entry_create failed to malloc bytes");
		exit(EXIT_FAILURE);
	}
	memcpy(entry->bytes, bytes, length);
	
	return entry;
}

void entry_free (struct entry * entry) {
	free(entry->bytes);
	free(entry);
}

/*
int main() {
	
	struct entry * e;
	char * raw_bytes = "123456789012345";
	const size_t test_length = 16;
	char test_bytes[16];

	for (int i = 0; i < test_length; i++) {
		test_bytes[i] = raw_bytes[i]-'0';
	}
	e = entry_create(test_length, test_bytes);
	for (int i = 0; i < test_length; i++) {
		printf("%c", e->bytes[i]+'0');
	}
	printf("\n");

	entry_free(e);
	return 0;
}
*/
