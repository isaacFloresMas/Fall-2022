#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"

void insert_sort(Node **list, uint32_t size) {
  // Going through each node in the list.
  for (uint32_t i = 0; i < size - 1; i++) {
    for (uint32_t j = i + 1; j < size; j++) {
      uint32_t temp = j;
      // If the current node has a smaller frequency than
      // the node before it, swap them.
      while (node_cmp(list[temp - 1], list[temp])) {
        Node *store = list[temp];
        list[temp] = list[temp - 1];
        list[temp - 1] = store;
        temp -= 1;
	// If no more nodes can be accessed, break.
        if (temp == 0)
          break;
      }
    }
  }
}

typedef struct PriorityQueue PriorityQueue;

struct PriorityQueue {
  uint32_t load; // The top of the priority queue.
  uint32_t capacity; // The capcity of the priority queue.
  Node **queue;  // The list of nodes.
};

PriorityQueue *pq_create(uint32_t capacity) {
  PriorityQueue *q = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  // Allocate memory for the priority queue and if successfully allocated,
  // set the load to 0 and allocate capcity nodes for the queue.
  if (q != NULL) {
    q->load = 0;
    q->capacity = capacity;
    q->queue = (Node **)calloc(capacity, sizeof(Node *));
  }
  if (!q->queue) {
    free(q);
    q = NULL;
  }
  // Return the new priority queue.
  return q;
}

// Frees the queue of nodes and the priority queue structure.
void pq_delete(PriorityQueue **q) {
  if (*q) {
    free((*q)->queue);
    free(*q);
    *q = NULL;
  }
}

// Returns true if the load is 0.
bool pq_empty(PriorityQueue *q) { return (q->load == 0); }

// Returns true if the load equals to the capcity.
bool pq_full(PriorityQueue *q) { return (q->load == q->capacity); }

// Returns the load size.
uint32_t pq_size(PriorityQueue *q) { return (q->load); }

bool enqueue(PriorityQueue *q, Node *n) {
  // If the queue is full, return false.
  if (pq_full(q))
    return false;
  // Add the given node to the next available position in the
  // priority queue. Then increment the load and insertion sort it.
  q->queue[q->load] = n;
  q->load += 1;
  insert_sort(q->queue, q->load);
  return true;
}

bool dequeue(PriorityQueue *q, Node **n) {
  // If the queue is empty, return false.
  if (pq_empty(q))
    return false;
  // Set the node passed in to the first index of the priority queue.
  // Then decrement the node.
  *n = q->queue[0];
  q->load -= 1;
  // Shift every node to the left to reorganize the priority queue.
  for (uint32_t i = 0; i < q->load; i++) {
    q->queue[i] = q->queue[i + 1];
  }
  return true;
}

// Debug priority queue print function.
void pq_print(PriorityQueue *q) {
  for (uint32_t i = 0; i < q->load; i++) {
    node_print(q->queue[i]);
  }
}
