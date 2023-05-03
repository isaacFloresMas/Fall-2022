#!/bin/bash

# Citations: I used the gnuplot manual for syntax to create the different plots. Used the manual pages for the commands uniq, sort, and head. I got the >> from a piazza discussion about writing data without deleting previous data on the file.

make clean && make collatz

for ((n = 2; n <= 10000; n += 1)); do ./collatz -n $n | sort -n -r | head -n 1 >> /tmp/collatz3.dat; ./collatz -n $n >> /tmp/collatz_e.dat; ./collatz -n $n | wc -l >> /tmp/collatz2.dat; done

# Figure 4 Graph
sort -n /tmp/collatz2.dat > /tmp/collatz_p.dat

uniq -c /tmp/collatz_p.dat > /tmp/collatz4.dat
# End of figure 4 graph

# Extra Graph
sort -n /tmp/collatz_e.dat > /tmp/collatz_pe.dat

uniq -c /tmp/collatz_pe.dat > /tmp/collatz5.dat
# End of extra graph

gnuplot <<END
    set terminal pdf
    set nokey
    set output "collatz.pdf"
    
    set xlabel "n"
    set ylabel "length"
    set xrange [0:10000]
    set yrange [0:300]
    set xtics 0,1000,10000
    set zeroaxis
    set title "Collatz Sequence Lengths"
    plot "/tmp/collatz2.dat" with dots
    
    set xlabel "n"
    set ylabel "value"
    set xrange [0:10000]
    set yrange [0:100000]
    set zeroaxis
    set title "Maximum Collatz Sequence Value"
    plot "/tmp/collatz3.dat" with dots
    
    set xlabel "length"
    set ylabel "frequency"
    set xrange [0:225]
    set yrange [0:200]
    set xtics 0,25,225
    set ytics 0,20,200
    set zeroaxis
    set title "Collatz Sequence Length Histogram"
    plot "/tmp/collatz4.dat" using 2:1 with steps 
    
    set xlabel "value"
    set ylabel "frequency"
    set xrange [0:100000]
    set yrange [0:5000]
    set xtics 0,10000,100000
    set ytic 0,1000,5000
    set zeroaxis
    set title "Collatz Sequence Value Histogram"
    plot "/tmp/collatz5.dat" using 2:1 with steps
END
    
rm /tmp/collatz2.dat
rm /tmp/collatz3.dat
rm /tmp/collatz_e.dat

echo "Done plotting!"
