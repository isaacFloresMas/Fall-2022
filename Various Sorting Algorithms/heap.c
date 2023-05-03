# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "mtrand.h"
# include "stats.h"

uint32_t l_child(uint32_t n) {
    return (2 * n + 1);
}

uint32_t r_child(uint32_t n) {
    return (2 * n + 2);
}

uint32_t parent(uint32_t n) {
    return ((n - 1) / 2);
}

void up_heap(Stats *stats, uint32_t *arr, uint32_t n) {
    uint32_t *up_heap = arr;
    while (n > 0 && (cmp(stats, up_heap[n], up_heap[parent(n)]) == -1))     // if the child is less than its parent, swap them
	{
	    swap(stats, (up_heap + n), (up_heap + parent(n)));
            n = parent(n);
	}
    return;	
}

void down_heap(Stats *stats, uint32_t *arr, uint32_t heap_size) {
    uint32_t n = 0;
    uint32_t smaller;
    uint32_t *sort_down = arr;
    while (l_child(n) < heap_size)
    {
	if (r_child(n) == heap_size) { // if the right child DNE set smaller as the left child
	    smaller = l_child(n);
	}
	else {
            smaller = ((cmp(stats, *(sort_down + l_child(n)), *(sort_down + r_child(n))) == -1)) ? l_child(n) : r_child(n);
	}
	if (cmp(stats, *(sort_down + n),  *(sort_down + smaller)) == -1) { // if the parent is less than the smaller child, break
	    break;
        }
	swap(stats, (sort_down + n), (sort_down + smaller)); // swap the parent with the smaller value
        n = smaller; // set the new parent as the smaller value
    }
    return;
}

void build_heap(Stats *stats, uint32_t *arr, uint32_t *heap_arr, uint32_t n_elements)
{
    uint32_t size = n_elements;
    uint32_t *heap = heap_arr;
    uint32_t *sortheap = arr;
    for (uint32_t n = 0; n < size; n += 1) //build the heap by up heaping the original array
    {
	*(heap + n) = move(stats, sortheap[n]);
	move(stats, heap[n]);
        up_heap(stats, heap, n);
    }
    return;
}

void heap_sort(Stats *stats, uint32_t *arr, uint32_t n_elements)
{
    uint32_t size = n_elements;
    uint32_t *sorth = arr;

    // builds the sorted array and heap array
    uint32_t *heap;
    heap = (uint32_t *)calloc(size, sizeof(uint32_t));
    uint32_t *sorted_list;
    sorted_list = (uint32_t *)calloc(size, sizeof(uint32_t));

    build_heap(stats, sorth, heap, size);
    for (uint32_t n = 0; n < size; n += 1)
    {
	sorted_list[n] = move(stats, heap[0]); // set the current sorted array index to the top of the heap
	heap[0] = move(stats, heap[size - n - 1]); // set the top of the heap to the last value of the heap
	move(stats, sorted_list[n]);
	move(stats, heap[size - n - 1]);
	down_heap(stats, heap, size - n); // down heap so that the top of the heap is the smallest value
    }
    for (uint32_t i = 0; i < size; i += 1)
        sorth[i] = sorted_list[i]; // set the original array to the sorted list
    free(heap);	// free the sorted array and heap array
    heap = NULL;
    free(sorted_list);
    sorted_list = NULL;
    return;
}
