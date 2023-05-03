# Assignment 2: A Small Numerical Library

The mathlib-test file imports functions from mathlib.c. Mathlib.c finds the approximations of sin(x), cos(x), arcsin(x), arccos(x), and arctan(x) through the use of taylor polynomials. It also computes natural log through the use of the Newton-Raphson method which computes the inverse of e^x. When a specific option is called through mathlib-test it increments an x value through a set domain depending on the function. The value for x is plugged into the mathlib.c version of the function and the math.h version of the function. Both values are compared to calculate the difference between them. This data is put into a table which is outputted in the terminal.

## Files to have in directory when running 'mathlib-test' program:

Ensure that the files **Makefile**, **mathlib.c**, and **mathlib-test.c** are in your current directory before running mathlib-test. 

## How to build mathlib-test:

Run the following to build the 'mathlib-test' program:

$ make

## How to run mathlib-test:

There are many options for running **mathlib-test**. After building the files, you
must run the file by writing:

 $ ./mathlib-test -(*option*). 
 
###### Options are:
 -a: to run all tests.
 -s: to run sin tests.
 -c: to run cos tests.
 -S: to run arcsin tests.
 -C: to run arccos tests.
 -T: to run arctan tests.
 -l: to run log tests.

## Issues with the program:

Arcsin(x) and arccos(x) are accurate to the 12th decimal place except for x = -1. At x = -1 the mathlib.c versions of arcsin(x) and arccos(x) are accurate up until the 7th decimal place compared to the math.h versions of them. The mathlib.c version of arctan(x) is different from math.h version starting at the 11th decimal place. The mathlib.c version of log(x) is different from the math.h version starting at the 12th decimal place.
