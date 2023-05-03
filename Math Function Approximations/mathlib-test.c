# include <stdio.h>
# include <math.h>
# include <unistd.h>
# include "mathlib.h"

int main(int argc, char **argv) {
    int opt = 0;
    double x;
    double pi = M_PI;
    int cs, cS, cc, cC, cT, cl;
    cs = cS = cc = cC = cT = cl = 0;

    while ((opt = getopt(argc, argv, "aSsCcTl")) != -1)
    {
	// if the option -a is chosen, display the tables for all functions
        if (opt == 'a')
        {  
            printf("%3s %14s %20s %17s\n", "x", "sin", "Library", "Difference");
	    printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            for (x = 0; x <= 2 * (pi); x = x + (0.05 * pi) )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_sin(x), sin(x), (my_sin(x) - sin(x)) );
                }
            
            printf("\n%3s %14s %20s %17s\n", "x", "cos", "Library", "Difference");
            printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            for (x = 0; x <= 2 * (pi); x = x + (0.05 * pi) )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_cos(x), cos(x), (my_cos(x) - cos(x)) );
                }
            
            printf("\n%3s %17s %17s %17s\n", "x", "arcsin", "Library", "Difference");
            printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            for (x = -1; x < 1; x = x + 0.05 )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_arcsin(x), asin(x), (my_arcsin(x) - asin(x)) );
                }

	    printf("\n%3s %17s %17s %17s\n", "x", "arccos", "Library", "Difference");
            printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            for (x = -1; x < 1; x = x + 0.05 )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_arccos(x), acos(x), (my_arccos(x) - acos(x)) );
                }
            
            printf("\n%3s %17s %17s %17s\n", "x", "arctan", "Library", "Difference");
            printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            for (x = 1; x < 10; x = x + 0.05 )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_arctan(x), atan(x), (my_arctan(x) - atan(x)));
                }
            
            printf("\n%3s %14s %20s %17s\n", "x", "log", "Library", "Difference");
            printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            for (x = 1; x < 10; x = x + 0.05 )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_log(x), log(x), (my_log(x) - log(x)));
                }
             break;
        }
	// if the option -s is chosen, plot the table for x, my version of sin, the library version, and the difference between the two from the domain of [0,2pi]
	else if (opt == 's' && cs == 0)
        {
	    printf("%3s %14s %20s %17s\n", "x", "sin", "Library", "Difference");
	    printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
	    cs += 1;
            for (x = 0; x <= 2 * (pi); x = x + (0.05 * pi) )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_sin(x), sin(x), (my_sin(x) - sin(x)) );
                }
        }
	// if the option -c is chosen, plot the table for x, my version of cos, the library version, and the difference between the two from the domain of [0,2pi]
        else if (opt == 'c' && cc == 0)
        {
            printf("%3s %14s %20s %17s\n", "x", "cos", "Library", "Difference");
            printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            cc += 1;
            for (x = 0; x <= 2 * (pi); x = x + (0.05 * pi) )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_cos(x), cos(x), (my_cos(x) - cos(x)) );
                }

        }
	// if the option -S is chosen, plot the table for x, my version of arcsin, the library version, and the difference between the two from the domain of [-1,1)
	else if (opt == 'S' && cS == 0)
        {
            printf("%3s %17s %17s %17s\n", "x", "arcsin", "Library", "Difference");
            printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            cS += 1;
            for (x = -1; x < 1; x = x + 0.05 )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_arcsin(x), asin(x), (my_arcsin(x) - asin(x)) );
                }
        }
	// if the option -C is chosen, plot the table for x, my version of arccos, the library version, and the difference between the two from the domain of [-1,1)
	else if (opt == 'C' && cC == 0)
        {
            printf("%3s %17s %17s %17s\n", "x", "arccos", "Library", "Difference");
            printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            cC += 1;
            for (x = -1; x < 1; x = x + 0.05 )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_arccos(x), acos(x), (my_arccos(x) - acos(x)) );
                }

        }
	// if the option -T is chosen, plot the table for x, my version of arctan, the library version, and the difference between the two from the domain of [1,10)
	else if (opt == 'T' && cT == 0)
        {
            printf("%3s %17s %17s %17s\n", "x", "arctan", "Library", "Difference");
            printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            cT += 1;
            for (x = 1; x < 10; x = x + 0.05 )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_arctan(x), atan(x), (my_arctan(x) - atan(x)));
                }
        }
	// if the option -l is chosen, plot the table for x, my version of log, the library version, and the difference between the two from the domain of [1,10)
	else if (opt == 'l' && cl == 0)
        {
            printf("%3s %14s %20s %17s\n", "x", "log", "Library", "Difference");
            printf("%3s %17s %17s %17s\n", "-", "------", "-------", "----------");
            cl += 1;
            for (x = 1; x < 10; x = x + 0.05 )
                {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my_log(x), log(x), (my_log(x) - log(x)));
                }
        }
    }
}
