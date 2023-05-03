#include "city.h"
#include "ll.h"
#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern uint32_t seeks; // Number of seeks performed.
extern uint32_t links; // Number of links traversed.

typedef struct HashTable HashTable;

struct HashTable {
  uint64_t salt;
  uint32_t size;
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_examined;
  bool mtf;
  LinkedList **lists;
};

HashTable *ht_create(uint32_t size, bool mtf) {
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  if (ht != NULL) {
    ht->mtf = mtf;
    ht->salt = 0x9846e4f157fe8840;
    ht->n_hits = ht->n_misses = ht->n_examined = 0;
    ht->n_keys = 0;
    ht->size = size;
    ht->lists = (LinkedList **)calloc(size, sizeof(LinkedList *));
    if (!ht->lists) {
      free(ht);
      ht = NULL;
    }
  }
  return ht;
}

void ht_delete(HashTable **ht) {
  for (uint32_t index = 0; index < (*ht)->size; index += 1) { // for each linked list
    if ((*ht)->lists[index] != NULL) // if the linked list has been created
      ll_delete(&((*ht)->lists[index])); // delete it
  }
  free((*ht)->lists); // free the linked list array
  (*ht)->lists = NULL;
  free(*ht); // free the hash table
  *ht = NULL;
}

// returns the size of the hash table
uint32_t ht_size(HashTable *ht) { return ht->size; }

Node *ht_lookup(HashTable *ht, char *oldspeak) {
  uint64_t index = hash(ht->salt, oldspeak);
  index = index % ht->size; // hash the oldspeak and modulo it by the size
  uint32_t start_seeks;
  uint32_t start_links;
  uint32_t updated_links;
  Node *curr = NULL;
  ll_stats(&start_seeks, &start_links); // get the links before looking up
  // if the linked list exists and oldspeak is in that linked list
  if (ht->lists[index] != NULL &&
      (curr = ll_lookup(ht->lists[index], oldspeak)) != NULL) {
    ll_stats(&start_seeks, &updated_links); // get the updated links after looking up
    // set the links examined to itself + (updated links - starting links)
    ht->n_examined += (updated_links - start_links);
    ht->n_hits += 1; // add 1 to the hash table hits
    return curr; // return the node with oldspeak in it
  }		
  else { // if the linked list doesn't exist and/or oldspeak isn't in that linked list
    ll_stats(&start_seeks, &updated_links); // get the updated links after looking up
    // set the links examined to itself + (updated links - starting links)
    ht->n_examined += (updated_links - start_links);
    ht->n_misses += 1; // add 1 to the hash table misses
    return curr;
  }
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
  uint64_t index = hash(ht->salt, oldspeak);
  index = index % ht->size; // hash the oldspeak and modulo it by the size
  if (ht->lists[index] == NULL) { // if the linked list doesn't already exist
    ht->lists[index] = ll_create(ht->mtf); // create the linked list
    ll_insert(ht->lists[index], oldspeak, newspeak); // insert the oldspeak & newspeak
    ht->n_keys += 1;
  } 
  // if the linked list already exists and the oldspeak is already inside
  else if (ll_lookup(ht->lists[index], oldspeak) != NULL) {
  }
  else { // if the linked list already exists and the oldspeak is not inside it
    ll_insert(ht->lists[index], oldspeak, newspeak); // insert the oldspeak & newspeak
    ht->n_keys += 1;
  }
}

uint32_t ht_count(HashTable *ht) {
  uint32_t count = 0;
  for (uint32_t index = 0; index < ht->size; index += 1) { // for each linked list
    if (ht->lists[index] != NULL) // if the linked list exists, add 1 to count
      count += 1;
  }
  return count;
}

void ht_print(HashTable *ht) {
  for (uint32_t index = 0; index < ht->size; index += 1) { // for each linked list
    if (ht->lists[index] != NULL) // if the linked list exists, print it
      ll_print(ht->lists[index]);
  }
}

// set the parameters to the hash table statistics
void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {
  *nk = ht->n_keys;
  *nh = ht->n_hits;
  *nm = ht->n_misses;
  *ne = ht->n_examined;
}
