
# Plot Root Locus
set title "Root Locus"         
set xlabel "Real Part, rad/sec"
set ylabel "Imaginary Part, rad/sec"
set grid
set nokey
set data style points
plot "RootLocus" using 1:2, "" using 3:4, "" using 5:6, "" using 7:8


