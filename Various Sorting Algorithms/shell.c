# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "mtrand.h"
# include "stats.h"
# include "set.h"


// Creates an array of each gap number
uint32_t gap(uint32_t n, uint32_t *gap_arr)
{  
    uint32_t bytes = sizeof(uint32_t);
    uint32_t element  = 0;
    for (element = 0; n > 1; element += 1)
    {   
	n = (n <= 2) ? 1 : (5*n) / 11;
	*(gap_arr + element) = n;
	bytes += sizeof(uint32_t);
	gap_arr = (uint32_t *)(realloc(gap_arr, bytes));                  // allocates more memory for the gap array
    }
    return element;
}

void shell_sort(Stats *stats, uint32_t *arr, uint32_t n_elements)
{
    uint32_t size = n_elements;
    uint32_t *sortsh = arr;
    uint32_t *gap_numbers;
    gap_numbers = (uint32_t *)malloc(sizeof(uint32_t));
    uint32_t count = gap(size, gap_numbers);

    for (uint32_t step_index = 0; step_index < count; step_index += 1)    // for each gap number
    {
        uint32_t gap = gap_numbers[step_index];
	for (uint32_t i = gap; i < size; i += 1)                          // first element to compare is i
	{
	    uint32_t j = i;
	    uint32_t temp = move(stats, sortsh[i]);
	    while (j >= gap && cmp(stats, temp, sortsh[j - gap]) == -1)   // second element to compare is (j - gap)
	    {								  // if the ith element is less than the number gap elements before it, swap them
		sortsh[j] = move(stats, sortsh[j - gap]);
		move(stats, sortsh[j]);
		j -= gap;
	    }
	    sortsh[j] = move(stats, temp);
	    move(stats, sortsh[j]);
	}
    }
    free(gap_numbers);                                                    // free the gap number array
    gap_numbers = NULL;
    return;
}
