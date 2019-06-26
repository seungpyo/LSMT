#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t error_t; 
#define ERROR_OK 0;
#define ERROR_FAIL 1;
#define ERROR_NYI 2;

struct bst_node {
	uint64_t key;
	uint64_t val;
	struct bst_node *left;
	struct bst_node *right;
};

struct bst_node* bst_create_node(uint64_t key, uint64_t val);
void bst_copy_node(struct bst_node *dst, struct bst_node *src);
struct bst_node* bst_insert(struct bst_node *root, struct bst_node *node);
error_t bst_delete(struct bst_node *root, uint64_t key);
error_t bst_find(struct bst_node *root, uint64_t key);

struct bst_node* bst_create_node(uint64_t key, uint64_t val) {
	struct bst_node * node = malloc(sizeof(struct bst_node));
	node->key = key;
	node->val = val;
	node->left = node->right = NULL;
	return node;
}
void bst_copy_node(struct bst_node *dst, struct bst_node *src) {
	dst->key = src->key;
	dst->val = src->val;
	dst->left = src->left;
	dst->right = src->right;
}
// Inserts a new node into a BST.
// Returns a root node.
struct bst_node* bst_insert(struct bst_node *root, struct bst_node *node) {
	struct bst_node *cur = root;
	if (!cur) {
		// Empty tree.
		root = bst_create_node(node->key, node->val);
		return root;
	}

	while (1) {
		if (cur->left && cur->right) {
			// Both children exist.
			cur = node->key < cur->key ? cur->left : cur->right;
		} else if (cur->left && !cur->right) {
			// No right child.
			if (node->key < cur->key) {
				cur = cur->left;
			} else {
				cur->right = node;
				break;
			}
		} else if (!cur->left && cur->right) {
			// No left child.
			if (cur->key < node->key) {
				cur = cur->right;
			} else {
				cur->left = node;
				break;
			}
		} else {
			// Leaf node.
			if (node->key < cur->key) {
				cur->left = node;
			} else {
				cur->right = node;
			}
			break;
		}
	}
	return root;

}

error_t bst_delete(struct bst_node *root, uint64_t key) {
	return ERROR_NYI;
}

error_t bst_find(struct bst_node *root, uint64_t key) {
	return ERROR_NYI;
}

void bst_print(struct bst_node *root) {
	if (!root) {
		return;
	}
	struct bst_node *cur = root;
	bst_print(root->left);
	printf("(%llu, %llu)\n", root->key, root->val);
	bst_print(root->right);
}

/**
  * prnq_size : # of elements in print queue
  * prnq_max : Maximum capacity of print queue
  */

void bst_print_queue(struct bst_node ** prnq, int prnq_size, int prnq_max) {
	if (!prnq) {
		return;
	}

	int depth = 0;
	while (prnq_size > 0) {

		if (prnq_max < prnq_size) {
			// Refill. Test passed.
			struct bst_node ** prnq_src = prnq;
			prnq_max *= 4;
			prnq = malloc(sizeof(struct bst_node*) * prnq_max);
			for (int i = 0; i < prnq_size; i++) {
				prnq[i] = prnq_src[i];
			}
			free(prnq_src);
		}
		
		int src_prnq_size = prnq_size;
		printf("depth = %d\n", depth++);
		for (int i = 0 ; i < src_prnq_size; i++) {
			printf("(%llu, %llu) ", prnq[i]->key, prnq[i]->val);
			if (prnq[i]->left) {
				prnq[prnq_size++] = prnq[i]->left;
			}
			if (prnq[i]->right) {
				prnq[prnq_size++] = prnq[i]->right;
			}
		}
		
		int prnq_cnt = 0;
		for (int i = src_prnq_size; i < prnq_size; i++, prnq_cnt++) {
			prnq[i - src_prnq_size] = prnq[i];
		}
		prnq_size = prnq_cnt;
		printf("\n");
	}
}

// Test driver.
int main() {
	struct bst_node * root = NULL;

	for (int i = 0; i < 5; i++) {
		struct bst_node * node = bst_create_node(i*2, i*2); 
		root = bst_insert(root, node);
	}

	for (int i = 0; i < 5; i++) {
		struct bst_node * node = bst_create_node(i*2+1, i*2+1); 
		root = bst_insert(root, node);
	}

	// bst_print(root);
	#define PRNQ_MAX 1
	struct bst_node ** prnq = malloc(sizeof(struct bst_node*) * PRNQ_MAX);
	prnq[0] = root;
	bst_print_queue(prnq, 1, PRNQ_MAX);

	return 0;
}
