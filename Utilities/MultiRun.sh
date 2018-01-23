# This script submits multiple runs to run concurrently.
# OSX distributes the load among the available CPU cores.
# While untested, Linux might do something similar.
# Usage:  ./Utilities/MultiRun.sh InOut1 InOut2 InOut3
# From Matlab or octave:
# system('./Utilities/MultiRun.sh InOut1 InOut2 InOut3')
# The folders InOut1, InOut2, etc must already exist

while([ -n "$1" ]); do
   ./42 $1 > $1/log.txt 2>&1 &
   shift
done

wait
echo "All runs complete"
