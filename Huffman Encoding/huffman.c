#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "code.h"
#include "defines.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

Node *build_tree(uint64_t hist[static ALPHABET]) {
  PriorityQueue *huff = pq_create(ALPHABET);
  // Create a priority queue and add a node to it for each
  // character in the histogram that has a frequency greater than 0.
  for (int i = 0; i < ALPHABET; i++) {
    if (hist[i] > 0) {
      enqueue(huff, node_create((uint8_t)i, hist[i]));
    }
  }
  Node *left;
  Node *right;
  Node *parent;
  // While the priority queue has more than 1 node in it.
  while (pq_size(huff) > 1) {
    // Dequeue two nodes with the first being the left node
    // and the second being the right node.
    dequeue(huff, &left);
    dequeue(huff, &right);
    // Join both nodes and enqueue the parent.
    parent = node_join(left, right);
    enqueue(huff, parent);
  }
  // Dequeue the last node which is the root, delete the priority
  // queue and return the root node.
  Node *root;
  dequeue(huff, &root);
  pq_delete(&huff);
  return root;
}

static Code c;

void build_codes(Node *root, Code table[static ALPHABET]) {
  // if the code is empty, initialize it.
  if (code_empty(&c))
    c = code_init();

  uint8_t temp_bit;
  // If the current node is not NULL.
  if (root) {
    // If the current node is a leaf node, set its symbol's index
    // in the code table to the current code.
    if (root->left == NULL && root->right == NULL)
      table[root->symbol] = c;
    else {
      // Push a 0 to the code and recurse from the current node's
      // left child. Once it has retuned, pop a bit.
      code_push_bit(&c, 0);
      build_codes(root->left, table);
      code_pop_bit(&c, &temp_bit);
      // Push a 1 to the code and recurse from the current node's
      // right child. Once it has retuned, pop a bit.
      code_push_bit(&c, 1);
      build_codes(root->right, table);
      code_pop_bit(&c, &temp_bit);
    }
  }
}

void dump_tree(int outfile, Node *root) {
  // If the current node isn't NULL.
  if (root) {
    // Recurse from the current node's left child and right child.
    dump_tree(outfile, root->left);
    dump_tree(outfile, root->right);
    // If the current node is a leaf, write an 'L' followed
    // by the current node's symbol to the outfile.
    if (root->left == NULL && root->right == NULL) {
      write_bytes(outfile, (uint8_t *)("L"), 1);
      write_bytes(outfile, &root->symbol, 1);
    }
    // Else, write an 'I' to the outfile. 
    else {
      write_bytes(outfile, (uint8_t *)("I"), 1);
    }
  }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
  // Create a stack.
  Stack *arbol = stack_create(nbytes);
  Node *left;
  Node *right;
  Node *parent;
  // For each element in the tree array.
  for (uint16_t i = 0; i < nbytes; i++) {
    // If the current element is an 'L', push the next element as a node
    // onto the stack and increment i.
    if (tree[i] == 'L') {
      stack_push(arbol, node_create(tree[i + 1], 0));
      i += 1;
    }
    // Else if the current element is an 'I', pop two nodes from the stack with
    // the first being the right node and the second being the left node. 
    // Then join the two nodes and push the joined node to the stack.
    else if (tree[i] == 'I') {
      stack_pop(arbol, &right);
      stack_pop(arbol, &left);
      parent = node_join(left, right);
      stack_push(arbol, parent);
    }
  }
  // Pop the last node from the stack, then delete the stack.
  // Return the popped node since it is the root of the rebuilt tree.
  Node *root;
  stack_pop(arbol, &root);
  stack_delete(&arbol);
  return root;
}

// Post-order traversal starting from the root and
// free the node at the end.
void delete_tree(Node **root) {
  if (*root) {
    delete_tree(&(*root)->left);
    delete_tree(&(*root)->right);
    node_delete(root);
  }
}
