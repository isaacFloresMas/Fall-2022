#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Citations: Based string length and duplicate code off of the strcmp
// and memcpy source code.

typedef struct Node Node;

char *duplicate(char *s);
uint64_t str_length(char *s);

struct Node {
  char *oldspeak;
  char *newspeak;
  Node *next;
  Node *prev;
};

Node *node_create(char *oldspeak, char *newspeak) {
  Node *n = (Node *)malloc(sizeof(Node)); // allocate memory for the node
  if (n) { // if successfully allocated
    n->oldspeak = duplicate(oldspeak); // set the nodes oldspeak to the given oldspeak
    n->newspeak = duplicate(newspeak); // set the nodes newspeak to the given newspeak
    n->next = NULL;
    n->prev = NULL;
  }
  return n; // return the new node
}

void node_delete(Node **n) {
  if (*n) { // if the node exists
    free((*n)->oldspeak); // free the memory for the duplicated oldspeak
    free((*n)->newspeak); // free the memory for the duplicated newspeak
    free(*n); // free the node
    *n = NULL;
  }
}

void node_print(Node *n) {
  if (n->newspeak == NULL) { // if badspeak
    printf("%s\n", n->oldspeak); // print the badspeak
  } else { // if oldspeak
    printf("%s -> %s\n", n->oldspeak, n->newspeak); // print the oldspeak & newspeak
  }
}

// duplicates a string
char *duplicate(char *s) {
  if (s == NULL) { // if s is NULL, return NULL
    return NULL;
  }
  // get the length of the string and add 1 to account for the '\0'
  uint64_t length = str_length(s) + 1;
  char *copy = (char *)malloc(length); // allocate memory for the duplicate
  for (uint64_t character = 0; character < length; character += 1) { // for each character
    *(copy + character) = *(s + character); // copy the current character to the duplicate
  }
  return copy; // return the copy
}

// returns the length of a string
uint64_t str_length(char *s) {
  uint64_t length;
  // for each character until the NULL ending character, add 1 to the length
  for (length = 0; *(s + length) != '\0'; length += 1) { 
  }
  return length; // return the length
}
