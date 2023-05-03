#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Citations: Based string compare code off of the strcmp source code.

uint64_t seeks; // Number of seeks performed.
uint64_t links; // Number of links traversed.

typedef struct LinkedList LinkedList;

bool str_compare(char *s, char *w);

struct LinkedList {
  uint32_t length;
  Node *head; // Head sentinel node.
  Node *tail; // Tail sentinel node.
  bool mtf;
};

LinkedList *ll_create(bool mtf) {
  // allocate memory for the linked list
  LinkedList *ll = (LinkedList *)malloc(sizeof(LinkedList));
  ll->length = 0;
  ll->mtf = mtf;
  char *temp = NULL;
  ll->head = node_create(temp, temp); // create the head sentinel
  ll->tail = node_create(temp, temp); // create the tail sentinel
  ll->head->next = ll->tail; // set the head's next node to the tail
  ll->tail->prev = ll->head; // set the tail's previous node to the head
  return ll; // return the new linked list
}

void ll_delete(LinkedList **ll) {
  // for each node in the linked list
  for (Node *curr = (*ll)->head; curr != NULL;) {
    Node *temp = curr->next; // store the current node's next node
    node_delete(&curr); // free the current node
    curr = temp; // set the current node to the stored node
  }
  free(*ll); // free the linked list
  *ll = NULL;
}

// return the length of the linked list
uint32_t ll_length(LinkedList *ll) { return ll->length; }

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
  seeks += 1; // add 1 to the seeks
  // for each node that isn't the head or tail sentinel nodes
  for (Node *curr = ll->head->next; curr != ll->tail; curr = curr->next) {
    links += 1; // add 1 to the links searched
    // if the current nodes oldspeak matches the oldspeak we're looking for & mtf is true
    if (str_compare(curr->oldspeak, oldspeak) == true && ll->mtf == true) {
      // if the current node is already in the front, don't move-to-front
      if (curr == ll->head->next) {
	return curr;
      }
      else {
        // set the current node's previous node's next node to the current node's next node
        curr->prev->next = curr->next;
        // set the current node's next node's previous node to the current node's previous node
        curr->next->prev = curr->prev;
        // set the current node's previous node to the head
        curr->prev = ll->head;
        // set the current node's next node to the head's next node
        curr->next = ll->head->next;
	// set the head's next node's previous node to the current node
	ll->head->next->prev = curr;
        // set the head's next node to the current node
        ll->head->next = curr;
        return curr; // return the current node
      }
		   
      // if the current nodes oldspeak matches the oldspeak we're looking for & mtf is false
    } else if (str_compare(curr->oldspeak, oldspeak) == true &&
               ll->mtf == false) {
        return curr; // return the current node
    }
  }
  return NULL; // if the a node with the given oldspeak can't be found, return NULL
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
  if (ll_lookup(ll, oldspeak) == NULL) { // if oldspeak isn't already in a node
    // create a new node with the given oldspeak and newspeak
    Node *node = node_create(oldspeak, newspeak); 
    // set the new node's next node to the head's next node
    node->next = ll->head->next; 
    // set the new node's previous node to the head
    node->prev = ll->head;
    // set the new node's next node's previous node to the new node
    node->next->prev = node;
    // set the head's next node to the new node
    ll->head->next = node;
    ll->length += 1; // add 1 to the linked list length
  }
}

void ll_print(LinkedList *ll) {
  // for each node that isn't the head or tail sentinel, print it
  for (Node *curr = ll->head->next; curr->next != NULL; curr = curr->next) {
    node_print(curr);
  }
}

// set the parameters to the number of seeks and links
void ll_stats(uint32_t *n_seeks, uint32_t *n_links) {
  *n_seeks = seeks;
  *n_links = links;
}

// comapres two strings, returns true if they're the same & false if different
bool str_compare(char *s, char *w) {
  // for each character in the both strings until one has finished
  for (uint64_t index = 0; *(s + index) != '\0' || *(w + index) != '\0';
       index += 1) {
    if (*(s + index) != *(w + index)) // if the current characters are not equal
      return false;
  }
  return true; // return true if the strings are equal
}
