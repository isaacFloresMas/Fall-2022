#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node {
  Node *left;         // Pointer to left child.
  Node *right;        // Pointer to right child.
  uint8_t symbol;     // Node's symbol.
  uint64_t frequency; // Frequency of symbol.
};

Node *node_create(uint8_t symbol, uint64_t frequency) {
  Node *n = (Node *)malloc(sizeof(Node)); // Allocate memory for the node.
  if (n) { // If successfully allocated.
    n->symbol = symbol; // Set it's symbol.
    n->frequency = frequency; // Set it's frequency.
    n->left = NULL;
    n->right = NULL;
  }
  return n; // Return the new node.
}

void node_delete(Node **n) {
  if (*n) {   // If the node exists.
    free(*n); // Free the node and set it's pointer to NULL.
    *n = NULL;
  }
}

Node *node_join(Node *left, Node *right) {
  // Set the parent's frequency to both node's frequency's combined.
  uint64_t sum = left->frequency + right->frequency;
  Node *parent = node_create('$', sum);
  // Set the parent's children to the given node's.
  parent->left = left;
  parent->right = right;
  // Return the new parent node.
  return parent;
}

// Return true if node n's frequency is larger than node m's.
bool node_cmp(Node *n, Node *m) { return (n->frequency > m->frequency); }

// Debug node print functions.
void node_print(Node *n) {
  printf("%c %lu\n", n->symbol, n->frequency);
}

void node_print_sym(Node *n) { printf("symbol: %c\n", n->symbol); }
