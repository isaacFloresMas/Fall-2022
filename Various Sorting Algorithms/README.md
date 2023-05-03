# Assignment 4: All Sorts of C Code

The sorting code can sort an n sized array with 4 different sorting methods being bubble sort, quick sort, heap sort, and shell sort. The user can use any of the sorting types. The user also has the option to print out p elements of the sorted array. The file prints out the sorting method used, the size of the array, and the number of moves and compares needed to complete the sort.

## Files to have in directory when running 'sorting' program:

Ensure that the files **Makefile**, **bubble.c**, **bubble.h**, **heap.c**, **heap.h**, **shell.c**, **shell.h**, **quick.c**, **quick.h**, **set.c**, **set.h**, **stats.c**, **stats.h**, **mtrand.c**, **mtrand.h**, and **sorting.c** are in your current directory before running sorting. 

Makefile compiles and builds the entire program. Sorting.c takes the parameters for each sorting method and prints the result. Bubble.c, shell.c, quick.c, and heap.c are the files that contain the code that completes each sorting algorithm. Mtrand.c is a random number generator. Set.c and stats.c are helper functions that keep track of user input and algorithm statistics. Each .h file allow different files to be used in separate files.

## How to build sorting:

Run the following to build the 'sorting' program:

$ make
or
$ make all
or
$ make sorting

If you want to remove any .o and any executable files run:

$ make clean

## How to run sorting:

There are many options for running **sorting**. After building the files, you
must run the file by writing the following in a terminal:

 $ ./sorting -(*option*). 
 
##### Options are:

• -a : Employs all sorting algorithms.

• -s : Enables Shell Sort.

• -b : Enables Bubble Sort.

• -q : Enables Quick Sort.

• -h : Enables Heap Sort.

• -r seed : Set the random seed to seed. The default seed is 13371453.

• -n size : Set the number of elements in the array to size. The default size is 100. The range of valid input is 1 ≤ n ≤ 250, 000, 000 where n is the size specified.

• -p elements : Print out p number of elements from the array. The default number of elements to print out is 100.

• -H : Prints out program usage.
