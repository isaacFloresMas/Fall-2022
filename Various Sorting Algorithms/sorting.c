# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "mtrand.h"
# include "stats.h"
# include "set.h"
# include "bubble.h"
# include "shell.h"
# include "quick.h"
# include "heap.h"

int main(int argc, char **argv)
{
    int opt = 0;
    uint32_t size = 100;                       // default size
    uint64_t seed = 13371453;                  // default seed
    uint32_t p_elements = 100;                 // default p_elements
    Set options = set_empty();
    while ((opt = getopt(argc, argv, "abhqsHr:n:p:")) != -1)
    {
	switch (opt) 
	{    
	case 'H':
	{   
	    // if the user wants help	
	    void print_help(void);
            print_help();
            return 0;
	}
	case 'a':
	{
	    options = set_insert(options, 1);
	    options = set_insert(options, 2);
	    options = set_insert(options, 3);
	    options = set_insert(options, 4);
	    break;
	}
	case 'b':
	{
            options = set_insert(options, 1);   // set value of 1 means bubble sort
	    break;
        }
	case 'h':
	{
	    options = set_insert(options, 2);   // set value of 2 means heap sort
	    break;
	}
	case 'q':
        {
            options = set_insert(options, 3);   // set value of 3 means quick sort
            break;
        }
	case 's':
        {
            options = set_insert(options, 4);   // set value of 4 means shell sort
            break;
        }
	case 'p':
	{
	    p_elements = atoi(optarg);
	    break;
	}
	case 'r':
	{
	    seed = strtoul(optarg, NULL, 0);
	    break; 
	}
	case 'n':
	{
	    if (atoi(optarg) < 1 || atoi(optarg) > 250000000) {
                return 1; // if the size is less than 1 or greater than 250000000 return non-zero error code
            }
	    else {
	        size = atoi(optarg);
	        break;
	    }
	}
	default:
	{
            // user gave invalid option so print help and exit
	    void print_help(void);
            print_help();
	    return 1;
	}
        }
    }	
        Stats stat;
	p_elements = (p_elements > size) ? size : p_elements;             // if p_elements is greater than size, set it to size else leave it as is
	void build_array(uint32_t *array, uint32_t size, uint64_t seed);  // build the random array

	for (int i = 1; i < 5; i += 1)
	{
	    if (set_member(options, i) && i == 1) {                       // if user wants bubble sort meaning 1 is in the set
		reset(&stat);
		uint32_t *sort_arr;
                sort_arr = (uint32_t *)calloc(size, sizeof(uint32_t));
                build_array(sort_arr, size, seed);
		bubble_sort(&stat , sort_arr, size);
		printf("Bubble Sort, %u elements, %lu moves, %lu compares", size, stat.moves, stat.compares);
		for (uint32_t e = 0; e < p_elements; e+=1) {              // print out p_elements if users wants them
	        if (e % 5 == 0)
	                printf("\n");
		    printf("%13u", sort_arr[e]);
                }
		printf("\n");
		free(sort_arr);
                sort_arr = NULL;
            }	
	    
	    else if (set_member(options, i) && i == 2) {                  // if user wants heap sort meaning 2 is in the set
		reset(&stat);
		uint32_t *sort_arr;
                sort_arr = (uint32_t *)calloc(size, sizeof(uint32_t));
                build_array(sort_arr, size, seed);
                heap_sort(&stat , sort_arr, size);
                printf("Heap Sort, %u elements, %lu moves, %lu compares", size, stat.moves, stat.compares);
                for (uint32_t e = 0; e < p_elements; e+=1) {              // print out p_elements if users wants them
                    if (e % 5 == 0)
                        printf("\n");
                    printf("%13u", sort_arr[e]);
                }
                printf("\n");
		free(sort_arr);
                sort_arr = NULL;
	    }
	    else if (set_member(options, i) && i == 3) {
		reset(&stat);
		uint32_t *sort_arr;
                sort_arr = (uint32_t *)calloc(size, sizeof(uint32_t));    // if user wants quick sort meaning 3 is in the set
                build_array(sort_arr, size, seed);
                quick_sort(&stat , sort_arr, size);
                printf("Quick Sort, %u elements, %lu moves, %lu compares", size, stat.moves, stat.compares);
                for (uint32_t e = 0; e < p_elements; e+=1) {              // print out p_elements if users wants them
                   if (e % 5 == 0)
                       printf("\n");
                   printf("%13u", sort_arr[e]);
                }
                printf("\n");
		free(sort_arr);
                sort_arr = NULL;
            }
	    else if (set_member(options, i) && i == 4) {
		reset(&stat);
		uint32_t *sort_arr;
                sort_arr = (uint32_t *)calloc(size, sizeof(uint32_t));    // if user wants shell sort meaning 4 is in the set
                build_array(sort_arr, size, seed);
                shell_sort(&stat , sort_arr, size);
                printf("Shell Sort, %u elements, %lu moves, %lu compares", size, stat.moves, stat.compares);
                for (uint32_t e = 0; e < p_elements; e+=1) {              // print out p_elements if users wants them
                    if (e % 5 == 0)
                        printf("\n");
                    printf("%13u", sort_arr[e]);
                }
                printf("\n");
		free(sort_arr);
                sort_arr = NULL;
            }
	}
	return 0;
}

// builds the random array of size elements
void build_array(uint32_t *array, uint32_t size, uint64_t seed)
{
    mtrand_seed(seed);
    for (uint32_t n = 0; n < size; n += 1)
        {
            array[n] += mtrand_rand64() & 0x3fffffff;
        }
}

// print help function
void print_help(void)
{
    printf("SYNOPSIS\n");
    printf("   A collection of comparison-based sorting algorithms.\n\n");
    printf("USAGE\n");
    printf("   ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]\n\n");
    printf("OPTIONS\n");
    printf("   -H              Display program help and usage.\n");
    printf("   -a              Enable all sorts.\n");
    printf("   -b              Enable Bubble Sort.\n");
    printf("   -h              Enable Heap Sort.\n");
    printf("   -q              Enable Quick Sort.\n");
    printf("   -s              Enable Shell Sort.\n");
    printf("   -n length       Specify number of array elements (default: 100).\n");
    printf("   -p elements     Specify number of elements to print (default: 100).\n");
    printf("   -r seed         Specify random seed (default: 13371453).\n");
}
