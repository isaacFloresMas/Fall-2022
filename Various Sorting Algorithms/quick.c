# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "mtrand.h"
# include "stats.h"
# include "shell.h"

// Citations: Based this code off of the quick sort example given in the The C Programming Language book,
//            but I changed it so that pivot is an approximate middle value rather than an actual middle value

void quick_sort(Stats *stats, uint32_t *arr, uint32_t n_elements)
{
    uint32_t size = n_elements;
    uint32_t *sortq = arr;
    uint32_t small = 8;
    uint32_t current_index = 0;

    // if the size is less than 8, shell sort
    if (size < small) {                                    
	shell_sort(stats , sortq, size);
        return;
    }

    // set pivot to the approximate middle value in the array
    uint32_t pivot = (sortq[0] + sortq[size / 2] + sortq[size - 1]) / 3;
    
    for (uint32_t i = 0; i < size; i += 1)
    {
	// if the current value is less then the left value, swap them
	if (cmp(stats, sortq[i],  pivot) == -1) {
	    swap(stats, (sortq + (current_index++)), (sortq + i));
	}
    }	
    // if nothing was swapped
    if (current_index == 0) {
        return;
    }

    // continue to quick sort the spliced arrays
    quick_sort(stats, sortq + 0, current_index);
    quick_sort(stats, sortq + current_index, size - current_index);
    return;
}
