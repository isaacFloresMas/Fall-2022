# include <stdio.h>
# include <math.h>
# include <unistd.h>
# include "mathlib.h"
# include <stdlib.h>

#define PI M_PI

// Citations: Used piazza posts that explain the methods on how to code sin(x) and arcsin(x). 

int factorial(int n);
double my_pow(double x, int y);
double square_root(double x);
double Exp(double x);

// Approximate the value of sin(x) through the Maclaurin series of sin(x)
double my_sin(double x)
{
    double base = x;
    double n = 1.0;
    double current = 1.0;
    double answ = base;
    double prev = base;
    double abs_check = 0.0;
    double base_squared = base * base;
    do
        {
	n += 2;
        current = -1 * prev * (base_squared / ((n-1) * n));
	prev = current;
	answ = answ + current;
	abs_check = (current > 0) ? current: current * -1;
        }
    while (abs_check >= 1e-11);
    return answ;
}

// Approximate the value of cos(x) through the Maclaurin series of cos(x)
double my_cos(double x)
{
    double base = x;
    double n = 0;
    double current = 1;
    double answ = 1;
    double prev = 1;
    double abs_check = 0;
    double base_squared = base * base;
    do
        {
	n += 2;
        current = -1 * prev * (base_squared / ((n-1) * n));
	prev = current;
        answ = answ + current;    
	abs_check = (current > 0) ? current: current * -1;
        }
    while (abs_check >= 1e-11);
    return answ;
}

// Approximate the value of arcsin(x) through the Maclaurin series of arcsin(x)
double my_arcsin(double x)
{
    double base = x;
    double current = 0.0;
    double prev = base;
    double compare = 0.0;
    double abs_check = 0.0;
    do
    {
        current = prev - ((my_sin(prev) - base) / my_cos(prev));
	compare = prev;
	prev = current;
        abs_check = prev - compare;
        abs_check = (abs_check < 0) ? abs_check * -1 : abs_check;
    }
    while (abs_check > 1e-11);
    return prev;
}

// Approximate the value of arccos(x) through the simple method of subtracting pi/2 by arcsin(x)
double my_arccos(double x)
{
    double base = x;
    return (PI / 2) - my_arcsin(base);
}

// Approximate the value of arctan(x) through the simple method of calculating arcsin( x / square_root( (x^2) + 1) ) 
double my_arctan(double x)
{
    double answ = 0;
    double base = x;
    answ = my_arcsin(base / square_root((base*base) + 1));
    return answ;
}

// Approximate the value of log(x) through the Newton-Raphson method
double my_log(double x)
{
    double base = x;
    double current = 0;
    double prev = 1;
    do
        {
        current = prev + ((base - Exp(prev)) / Exp(prev));
        prev = current;
        }
    while (Exp(current) - base > 1e-11);
    return current;
}


// Helper Functions
int factorial(int n)
{
    int start = n;
    int prev = n;
    if (prev == 0)
        {return 1;}
    else
    {
    start = (start != 1) ? n - 1: n;
    for (; start != 0; start -= 1)
        {
        prev = prev * start;
        }
    return prev;
    }
}

double my_pow(double x, int y)
    {
    double base = x;
    int power = 0;
    double prev = 1;
    double current = 0;
    for (power = y; power != 0; power -= 1)
        {
        current = prev * base;
        prev = current;
        }
    return prev;
    
    }

double square_root(double x) 
  {
  double m, l = 0.0;
  double h = (x < 1) ? 1 : x;
  double epsilon = 1e-10;
  double range = 0.0;
  do {
    range += 0;
    m = (l + h) / 2.0;
    if (m * m < x) {
      l = m;
    } else {
      h = m;
    }
    range = (l > h) ? l - h : h - l;
  } while (range > epsilon);
  return m;
  }

double Exp(double x) 
{
    double t = 1.0;
    double y = 1.0;
    double epsilon = 1e-10;
    for (double k = 1.0; t > epsilon; k += 1.0) {
    t *= x / k;
    y += t;
    }
    return y;
}
