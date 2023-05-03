# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "mtrand.h"
# include "stats.h"
# include "set.h"

void bubble_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
    uint32_t size = n_elements;
    uint32_t *sortb = arr;
    uint32_t j, i;
    for (i = 0; i < (size - 1); i += 1) {                            // for loop size-1 amount of times
	int not_swapped = 1;                                         // nothing has been swapped
	for (j = (size - 1); j != i; j -= 1) {                       // for every value in the array except for the last value starting from the top
	    if (cmp(stats, *(sortb + j), *(sortb + (j-1))) == -1) {  // if the current element is less the one before it, swap them
		swap(stats, (sortb + j), (sortb + (j-1)));            
		not_swapped = 0;                                     // something has been swapped
	    }
	}
	if (not_swapped)	                                     // if nothing has swapped, break
	    break;
    }
    return;
}
