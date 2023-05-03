#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"

typedef struct Stack Stack;

struct Stack {
  uint32_t top;
  uint32_t capacity;
  Node **items;
};

Stack *stack_create(uint32_t capacity) {
  Stack *s = (Stack *)malloc(sizeof(Stack));
  // Allocate memory for the stack then set the top to 0
  // and make the stack of capacity nodes.
  if (s != NULL) {
    s->top = 0;
    s->capacity = capacity;
    s->items = (Node **)calloc(capacity, sizeof(Node *));
  }
  // If the stack of nodes couldn't be allocated correctly,
  // free the stack.
  if (!s->items) {
    free(s);
    s = NULL;
  }
  // Return the stack. 
  return s;
}

void stack_delete(Stack **s) {
  // Free the stack of nodes and the stack structure.
  if (*s) {
    free((*s)->items);
    free(*s);
    *s = NULL;
  }
}

// Return true if the top is not 0.
bool stack_empty(Stack *s) {
  if (s->top != 0)
    return false;
  return true;
}

// Return true if the top equals to the capacity.
bool stack_full(Stack *s) {
  if (s->top == s->capacity)
    return true;
  return false;
}

// Returns the top size.
uint32_t stack_size(Stack *s) { return s->top; }

bool stack_push(Stack *s, Node *n) {
  // If the stack is full, return false.
  if (stack_full(s))
    return false;
  // Set the top location of the stack to the node passed in,
  // then increment the top.
  s->items[s->top] = n;
  s->top += 1;
  return true;
}

bool stack_pop(Stack *s, Node **n) {
  // If the stack is empty, return false.
  if (stack_empty(s)) 
    return false;
  // Decrement the top, and set the node passed in to the
  // node at the top of the stack.
  s->top -= 1;
  *n = s->items[s->top];
  // Removed the popped node.
  s->items[s->top] = 0;
  return true;
}

// Debug function for a stack.
void stack_print(Stack *s) {
  for (uint32_t i = 0; i < s->capacity; i++) {
    if (s->items[i] != 0)
      node_print(s->items[i]);
  }
}
