# This script submits multiple runs to run concurrently.
# OSX distributes the load among the available CPU cores.
# While untested, Linux might do something similar.
# Usage:  ./BatchRun.sh InOut1 InOut2 InOut3
# From Matlab or octave:
# system('./BatchRun.sh InOut1 InOut2 InOut3')
# The folders InOut1, InOut2, etc must already exist

cd ..
while([ -n "$1" ]); do
   ./42 ./MonteCarlo/$1 ./Model > ./MonteCarlo/$1/log.txt 2>&1 &
   shift
done

wait
echo "All runs complete"
cd MonteCarlo
